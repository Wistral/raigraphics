//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"
#include "glog/logging.h"
#include <FreeImage.h>
#include <thread>

namespace rai_graphics {

typedef void *(RAI_graphics::*Thread2Ptr)(void *);
typedef void *(*PthreadPtr)(void *);

RAI_graphics::RAI_graphics(int windowWidth, int windowHeight) {
  windowWidth_ = windowWidth;
  windowHeight_ = windowHeight;
  objectsInOrder_.push_back(nullptr);
}

RAI_graphics::~RAI_graphics() {

}

void RAI_graphics::start() {
  Thread2Ptr t = &RAI_graphics::loop;
  PthreadPtr p = *(PthreadPtr *) &t;
  pthread_create(&mainloopThread, 0, p, this);
  mtxLoop.lock();
}

void RAI_graphics::end() {
  mtxLoop.unlock();
  pthread_join(mainloopThread, NULL);
}

void *RAI_graphics::loop(void *obj) {
  display = new Display(windowWidth_, windowHeight_, "RAI simulator");
  camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), 70.0f, (float) windowWidth_ / (float) windowHeight_, 0.1f, 1000.0f);
  shader_basic = new Shader_basic;
  shader_flat = new Shader_flat;
  shader_background = new Shader_background;
  shader_mouseClick = new Shader_mouseClick;
  interactionArrow = new object::Arrow(0.2, 0.1, 1, 0.3);

  light = new Light;

  while (true) {
    if (mtx.try_lock()) {
      if (mtxLoop.try_lock()) break;
      watch.start();
      mtxinit.lock();
      init();
      mtxinit.unlock();
      draw();
      mtx.unlock();
      double elapse = watch.measure();
      if (terminate) break;
      usleep(std::max((1.0 / FPS_ - elapse) * 1e6, 0.0));
    }
  }

  for (auto *sob: supObjs_)
    sob->destroy();

  for (auto *ob: objs_)
    ob->destroy();

  if (checkerboard)
    checkerboard->destroy();

  if (background)
    background->destroy();

  interactionArrow->destroy();

  delete display;
  delete camera;
  delete shader_background;
  delete shader_basic;
  delete shader_flat;
  delete shader_mouseClick;
  delete interactionArrow;
}

void RAI_graphics::init() {
  if (background && backgroundChanged) background->init();
  if (checkerboard && checkerboardChanged) checkerboard->init();

  backgroundChanged = false;
  checkerboardChanged = false;

  mtxLight.lock();
  if (lightPropChanged) {
    light->setDiffuse(lightProp.diff_light);
    light->setAmbient(lightProp.amb_light);
    light->setSpecular(lightProp.spec_light);
    light->setPosition(lightProp.pos_light);
    lightPropChanged = false;
  }
  mtxLight.unlock();

  mtxCamera.lock();
  if (cameraPropChanged) {
    if (cameraProp.toFollow)
      camera->follow(cameraProp.toFollow, cameraProp.relativeDist);
    cameraPropChanged = false;
  }
  mtxCamera.unlock();

  for (auto *sob: added_supObjs_) {
    sob->init();
    supObjs_.push_back(sob);
  }

  for (auto *ob: added_objs_) {
    ob->init();
    objs_.push_back(ob);
  }

  for (auto sh: added_shaders_)
    switch (sh) {
      case object::RAI_SHADER_BASIC: shaders_.push_back(shader_basic);
        break;
      case object::RAI_SHADER_FLAT: shaders_.push_back(shader_flat);
        break;
      default: shaders_.push_back(shader_basic);
        break;
    }

  for (auto *ob: tobeRemoved_objs_) {
    ob->destroy();
    ptrdiff_t pos = find(objs_.begin(), objs_.end(), ob) - objs_.begin();
    objs_.erase(objs_.begin() + pos);
    shaders_.erase(shaders_.begin() + pos);
  }

  for (auto *sob: tobeRemoved_supObjs_) {
    sob->destroy();
    ptrdiff_t pos = find(supObjs_.begin(), supObjs_.end(), sob) - supObjs_.begin();
    supObjs_.erase(supObjs_.begin() + pos);
  }

  added_objs_.clear();
  added_shaders_.clear();
  added_supObjs_.clear();
  tobeRemoved_supObjs_.clear();
  tobeRemoved_objs_.clear();
}

void RAI_graphics::draw() {

  display->Clear(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

  /// draw obj with monotone for mouse click inputs
  for (auto *sob: supObjs_) {
    if (sob->isVisible()) {
      for (auto *ob: sob->getChildren()) {
        shader_mouseClick->Bind();
        shader_mouseClick->Update(camera, ob);
        ob->draw();
        shader_mouseClick->Bind();
      }
    }
    for (int i = 0; i < objs_.size(); i++) {
      if (!objs_[i]->isVisible()) continue;
      shader_mouseClick->Bind();
      shader_mouseClick->Update(camera, objs_[i]);
      objs_[i]->draw();
      shader_mouseClick->UnBind();
    }
  }


  /// UI
  static const int NO_OBJECT = 16646655;
  while (SDL_PollEvent(&e)) {
    int objId = NO_OBJECT;
    bool startInteraction=false, endInteraction=false;
    switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
        if (e.button.clicks == 2 && !keyboard()[RAI_KEY_LCTRL])
          objId = readObjIdx();
        else if (e.button.clicks == 1 && keyboard()[RAI_KEY_LCTRL] && readObjIdx()==highlightedObjId)
          startInteraction = isInteracting = true;
        break;
      case SDL_MOUSEBUTTONUP:
        isInteracting = false;
        endInteraction = true;
        break;
      case SDL_MOUSEWHEEL:
        if (e.wheel.y == 1)
          camera->zoomOut();
        else if (e.wheel.y == -1)
          camera->zoomIn();
        break;
      case SDL_KEYDOWN:
        if(keyboard()[RAI_KEY_ESCAPE] && highlightedObjId != NO_OBJECT)
          objectsInOrder_[highlightedObjId]->deHighlight();
        break;
    }

    if (objId != NO_OBJECT) {
      camera->follow(objectsInOrder_[objId]);
      if(highlightedObjId != NO_OBJECT)
        objectsInOrder_[highlightedObjId]->deHighlight();

      if(highlightedObjId == objId) {
        highlightedObjId = NO_OBJECT;
      } else {
        highlightedObjId = objId;
        objectsInOrder_[highlightedObjId]->highlight();
      }
    }

    if (startInteraction)
      SDL_GetMouseState(&interStartingX, &interStartingY);

    if (isInteracting) {
      int tx, ty;
      SDL_GetMouseState(&tx, &ty);
      tx -= interStartingX;
      ty -= interStartingY;
      Transform objTrans;
      glm::mat4 cameraT;
      glm::vec3 cameraPos, objectPos, diffPos, arrowEnd3;
      objectsInOrder_[highlightedObjId]->getTransform(objTrans);

      glm::vec4 arrowOrigin = objTrans.GetModel() * glm::vec4(objectsInOrder_[highlightedObjId]->com, 1);
      camera->GetVP(cameraT);
      camera->GetPos(cameraPos);
      diffPos = cameraPos - *objTrans.GetPos();
      float normDiff = glm::l2Norm(diffPos);
//      interactionArrow->setScale(normDiff/1000.0f*sqrt(float(tx*tx+ty*ty)));
      std::cout<<"scale "<<normDiff/1000.0f*sqrt(float(tx*tx+ty*ty))<<std::endl;

      glm::vec4 arrowEnd = glm::inverse(cameraT) * glm::vec4(tx,ty,0,1);
      arrowEnd3 = glm::normalize(glm::vec3(arrowEnd));
      auto axis = glm::cross(arrowEnd3, glm::vec3(1,0,0));
      axis = glm::normalize(axis);
      float cos = arrowEnd3[0];
      Eigen::Vector3d arrowPosE(arrowOrigin[0], arrowOrigin[1], arrowOrigin[2]);
//      interactionArrow->setPos(arrowPosE);
      Eigen::Matrix3d arrowRotE;
      glm::mat4 arrowRotglm = glm::rotate(float(std::acos(cos)), axis);
      arrowRotE << arrowRotglm[0][0], arrowRotglm[1][0], arrowRotglm[2][0],
                   arrowRotglm[0][1], arrowRotglm[1][1], arrowRotglm[2][1],
                   arrowRotglm[0][2], arrowRotglm[1][2], arrowRotglm[2][2];
      shader_basic->Bind();
      shader_basic->Update(camera, light, interactionArrow, false);
      interactionArrow->draw();
      shader_basic->UnBind();
    }
  }

  /// update camera with events
  camera->Control(e);
  camera->update();

  /// clear images that was generated for mouse clicks
  display->Clear(0,0,0,0);

  /// draw checkerboard and reflections
  if (checkerboard) {
    drawObj(true);
    checkerboard->draw(camera, light, checkerboard->reflectance, false);
  }

  /// draw the real objects
  drawObj(false);

  /// draw background
  if (background) {
    shader_background->Bind();
    shader_background->Update(camera, light, background);
    background->draw();
    shader_background->UnBind();
  }

  if (saveSnapShot) {
    if (imageCounter < 2e3) {
      areThereimagesTosave = true;
      std::string imageFileName = std::to_string(imageCounter++);
      while (imageFileName.length() < 7)
        imageFileName = "0" + imageFileName;
      GLubyte *pixels = new GLubyte[3 * windowWidth_ * windowHeight_];
      glReadPixels(0, 0, windowWidth_, windowHeight_, GL_BGR, GL_UNSIGNED_BYTE, pixels);
      FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels, windowWidth_, windowHeight_, 3 * windowWidth_, 24,
                                                     0xFF0000, 0x00FF00, 0x0000FF, false);
      FreeImage_Save(FIF_BMP, image, (image_dir + "/" + imageFileName + ".bmp").c_str(), 0);
      FreeImage_Unload(image);
      delete[] pixels;
    } else {
      LOG(FATAL) << "You are saving too many frames. RAI shutting down to prevent possible system breakdown";
    }
  }

  display->SwapBuffers();
}

void RAI_graphics::addObject(object::SingleBodyObject *obj, object::ShaderType type) {
  std::lock_guard<std::mutex> guard(mtxinit);
  LOG_IF(FATAL, !obj) << "the object is not created yet";
  added_objs_.push_back(obj);
  if (type == object::RAI_SHADER_OBJECT_DEFAULT)
    type = obj->defaultShader;
  added_shaders_.push_back(type);
  obj->setObIndex(++objectIdexToBeAssigned);
  objectsInOrder_.push_back(obj);
}

void RAI_graphics::addSuperObject(object::MultiBodyObject *obj) {
  std::lock_guard<std::mutex> guard(mtxinit);
  LOG_IF(FATAL, !obj) << "the object is not created yet";
  added_supObjs_.push_back(obj);
  for (auto &ob: obj->getChildren()) {
    ob->setObIndex(++objectIdexToBeAssigned);
    objectsInOrder_.push_back(ob);
  }
}

void RAI_graphics::addBackground(object::Background *back) {
  std::lock_guard<std::mutex> guard(mtxinit);
  backgroundChanged = true;
  LOG_IF(FATAL, !back) << "the object is not created yet";
  background = back;
}

void RAI_graphics::addCheckerBoard(object::CheckerBoard *back) {
  checkerboard = back;
  checkerboardChanged = true;
}

void RAI_graphics::removeObject(object::SingleBodyObject *obj) {
  std::lock_guard<std::mutex> guard(mtxinit);
  tobeRemoved_objs_.push_back(obj);
}

void RAI_graphics::removeSuperObject(object::MultiBodyObject *obj) {
  std::lock_guard<std::mutex> guard(mtxinit);
  tobeRemoved_supObjs_.push_back(obj);
}

void RAI_graphics::setBackgroundColor(float r, float g, float b, float a) {
  clearColor[0] = r;
  clearColor[1] = g;
  clearColor[2] = b;
  clearColor[3] = a;
}

void RAI_graphics::setLightProp(LightProp &prop) {
  std::lock_guard<std::mutex> guard(mtxLight);
  lightProp = prop;
  lightPropChanged = true;
}

void RAI_graphics::setCameraProp(CameraProp &prop) {
  std::lock_guard<std::mutex> guard(mtxCamera);
  cameraProp = prop;
  cameraPropChanged = true;
}

void RAI_graphics::savingSnapshots(std::string logDirectory, std::string fileName) {
  mtx.lock();
  image_dir = logDirectory;
  imageCounter = 0;
  videoFileName = fileName;
  saveSnapShot = true;
  mtx.unlock();
}

void RAI_graphics::images2Video() {
  std::cout << "saving video. This might take a few seconds" << std::endl;
  std::lock_guard<std::mutex> guard(mtxCamera);
  saveSnapShot = false;
  if (!areThereimagesTosave) return;
  areThereimagesTosave = false;
  Thread2Ptr t = &RAI_graphics::images2Video_inThread;
  PthreadPtr p = *(PthreadPtr *) &t;
  pthread_t tid;
  if (pthread_create(&tid, 0, p, this) == 0)
    pthread_detach(tid);

  while (mtx.try_lock())
    mtx.unlock();
}

void *RAI_graphics::images2Video_inThread(void *obj) {
  std::lock_guard<std::mutex> guard(mtx);
  std::string
    command = "ffmpeg -r 60 -i " + image_dir + "/%07d.bmp -s 800x600 -c:v libx264 -crf 5 " + image_dir + "/" + videoFileName + ".mp4 >nul 2>&1";
  int i = system(command.c_str());
  command = "rm -rf " + image_dir + "/*.bmp";
  i = system(command.c_str());
  return NULL;
}

const Uint8 *RAI_graphics::keyboard() {
  return SDL_GetKeyboardState(NULL);
}

const MouseInput *RAI_graphics::mouse() {
  SDL_Event e;
  SDL_PollEvent(&e);
  mouseInput.wheel = e.wheel;
  mouseInput.leftB = e.button.button;
  Uint32 mbuttonState = SDL_GetMouseState(&mouseInput.x, &mouseInput.y);

  if (mbuttonState == SDL_BUTTON_LEFT) mouseInput.leftB = true;
  else mouseInput.leftB = false;

  if (mbuttonState == SDL_BUTTON_RIGHT) mouseInput.rightB = true;
  else mouseInput.rightB = false;

  if (mbuttonState == SDL_BUTTON_MIDDLE) mouseInput.middleB = true;
  else mouseInput.middleB = false;

  return &mouseInput;
}

int RAI_graphics::readObjIdx() {
  int tmpx = 0, tmpy = 0;
  SDL_GetMouseState(&tmpx, &tmpy);
  float color[4];
  glReadPixels(tmpx, windowHeight_ - tmpy, 1, 1, GL_RGB, GL_FLOAT, color);
  return color[0] * 255 + color[1] * 255 * 255 + color[2] * 255 * 255 * 255;
}

void RAI_graphics::drawObj(bool isReflection) {
  for (auto *sob: supObjs_)
    if (sob->isVisible()) sob->draw(camera, light, 1.0, isReflection);

  for (int i = 0; i < objs_.size(); i++) {
    if (!objs_[i]->isVisible() || !objs_[i]->reflectable) continue;
    shaders_[i]->Bind();
    shaders_[i]->Update(camera, light, objs_[i], isReflection);
    objs_[i]->draw();
    shaders_[i]->UnBind();

    // draw ghost
    // TODO code refine
    objs_[i]->usingTempTransform(true);
    for (auto &ghost : objs_[i]->getGhosts()) {
      objs_[i]->setTempTransform(ghost);
      shaders_[i]->Bind();
      shaders_[i]->Update(camera, light, objs_[i], isReflection);
      objs_[i]->draw();
      shaders_[i]->UnBind();
    }
    objs_[i]->usingTempTransform(false);
  }

}

} // rai_graphics
