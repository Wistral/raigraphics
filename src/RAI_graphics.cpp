//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"
#include "raiCommon/math/RAI_math.hpp"
#include "raiCommon/rai_utils.hpp"
#include <FreeImage.h>
#include <thread>
#include <raiGraphics/obj/Sphere.hpp>
#include <SDL2/SDL_ttf.h>
#include <sstream> // stringstream
#include <iomanip>

#define TEXTMENUCOUNT 10

namespace rai_graphics {

typedef void *(RAI_graphics::*Thread2Ptr)(void *);
typedef void *(*PthreadPtr)(void *);

RAI_graphics::RAI_graphics(int windowWidth, int windowHeight) :
  menuTextToggle(TEXTMENUCOUNT, false), customToggle(10, false) {
  windowWidth_ = windowWidth;
  windowHeight_ = windowHeight;
  objectsInOrder_.push_back(nullptr);
  menuText.resize(TEXTMENUCOUNT);
  for (auto &mtxt: menuText) mtxt.resize(2);
  textBoard.resize(TEXTMENUCOUNT);
  font.resize(6);
  menuText[0][0] = "Show Keyboard Input F1";
  menuText[0][1] =
    " mouse          = camera \n ldblclk          = highlight object \n ctr+drag       = interaction1 \n alt+drag       = interaction2  \n space           = camera mode  \n shift+r          = videos On/Off  \n F1~F5          = user defined menus \n 1~5             = user defined toggles";

  /// menus
  for (auto &tb: textBoard) {
    tb = new object::Rectangle(windowWidth_, windowHeight_);
    tb->setTranslation(10, 10);
    tb->setTransparency(0.4);
  }

  textBoard[0]->setTextWrap(300);
  textBoard[5]->setTranslation(windowWidth_ - 97, windowHeight_ - 25);
  textBoard[5]->setTextWrap(windowWidth_ - 40);

  textBoard[6]->setTranslation(windowWidth_ - 205, 10);
  textBoard[6]->setTextWrap(windowWidth_ - 40);

  textBoard[8]->setTextWrap(windowWidth_ - 40); /// next to cursor

  keyboardEvent.resize(8, false);

  interactionForce.setZero();
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
  camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::radians(60.0f), (float) windowWidth_ / (float) windowHeight_, 0.01f, 10000.0f);
  cameraDepth_ = windowHeight_ / 2.0 / tan(30.0 / 180.0 * M_PI);
  light = new Light;
  shader_basic = new Shader_basic;
  shader_flat = new Shader_flat;
  shader_background = new Shader_background;
  shader_mouseClick = new Shader_mouseClick;
  shader_line = new Shader_line;
  shader_menu = new Shader_menu;
  shader_checkerboard = new Shader_checkerboard;
  interactionArrow = new object::Arrow(0.02, 0.04, 0.8, 0.2);
  interactionArrow->setColor({1, 0, 0});
  interactionBall = new object::Sphere(1, false);
  interactionBall->setColor({1, 0, 0});

  interactionArrow->init();
  interactionBall->init();

  TTF_Init();
  font[0] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 8);
  font[1] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 12);
  font[2] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 18);
  font[3] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 24);
  font[4] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 30);
  font[5] = TTF_OpenFont((std::string(std::getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/FreeSans.ttf").c_str(), 38);

  RAIFATAL_IF(font[0] == nullptr, "Could not find the font file. Run the install script provided.");

  /// menus
  for (auto &tb: textBoard)
    tb->init();

  textBoard[0]->setFontSize(2);
  textBoard[6]->setFontSize(2);
  textBoard[7]->setFontSize(2);
  textBoard[5]->setFontSize(2);

  for (int i = 0; i < TEXTMENUCOUNT; i++)
    textBoard[i]->writeText(font, menuText[i][0]);

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
      elapse = watch.measure();
      actualFPS_ = 1.0 / elapse;
      isGraphicsReady = true;
    }
  }

  for (auto *sob: supObjs_)
    sob->destroy();

  for (auto *ob: objs_)
    ob->destroy();

  for (auto *tb: textBoard)
    tb->destroy();

  if (checkerboard)
    checkerboard->destroy();

  if (background)
    background->destroy();

  interactionArrow->destroy();
  interactionBall->destroy();

  for (auto *tb: textBoard)
    delete tb;

  for (auto *fo: font)
    TTF_CloseFont(fo);

  for(auto* ln: lines_)
    delete ln;

  TTF_Quit();
  delete camera;
  delete light;
  delete shader_background;
  delete shader_basic;
  delete shader_flat;
  delete shader_line;
  delete shader_mouseClick;
  delete shader_menu;
  delete shader_checkerboard;
  delete interactionArrow;
  delete interactionBall;
  delete display;
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

  for (auto *ob: tobeRemovedAndDeleted_objs_) {
    if (camera->getToFollowObj() == ob)
      camera->unFollowOb();
    ob->destroy();
    ptrdiff_t pos = find(objs_.begin(), objs_.end(), ob) - objs_.begin();
    objs_.erase(objs_.begin() + pos);
    shaders_.erase(shaders_.begin() + pos);
    pos = find(objectsInOrder_.begin(), objectsInOrder_.end(), ob) - objectsInOrder_.begin();
    if (pos != objectsInOrder_.size()) objectsInOrder_[pos] = nullptr;
    delete ob;
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
  tobeRemovedAndDeleted_objs_.clear();
}

void RAI_graphics::draw() {

  /// clear images that was generated for mouse clicks
  display->Clear(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

  /// draw obj with monotone for mouse click inputs
  for (auto *sob: supObjs_)
    if (sob->isVisible()) {
      for (auto *ob: sob->getChildren()) {
        if (not ob->isVisible() or not ob->isSelectable()) continue;
        shader_mouseClick->Bind();
        shader_mouseClick->Update(camera, ob);
        ob->draw();
        shader_mouseClick->Bind();
      }
    }

  for (int i = 0; i < objs_.size(); i++) {
    if (not objs_[i]->isVisible() or not objs_[i]->isSelectable()) continue;
    shader_mouseClick->Bind();
    shader_mouseClick->Update(camera, objs_[i]);
    objs_[i]->draw();
    shader_mouseClick->UnBind();
  }

  /// UI
  static const int NO_OBJECT = 16646655;
  bool startInteraction = false;
  bool focus = false;
  int objId = NO_OBJECT;

  std::stringstream stream;
  stream << "FPS: " << std::setprecision(4) << actualFPS_;
  menuText[5][1] = stream.str();

  if (keyboard()[RAI_KEY_LCTRL]) {
    textBoard[6]->setTranslation(windowWidth_ - 205, 10);
    std::stringstream inStr;
    inStr << "Interaction Strength: " << std::setprecision(2) << interactionMagnitude;
    menuText[6][0] = inStr.str();
  } else if (keyboard()[RAI_KEY_T]) {
    textBoard[6]->setTranslation(windowWidth_ - 240, 10);
    std::stringstream inStr;
    inStr << "Simulation RealTimeFtr: " << std::setprecision(2) << realtimeFactor;
    menuText[6][0] = inStr.str();
  } else {
    menuText[6][0] = "";
  }

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:isQuiting = true;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (e.button.clicks == 2 && !keyboard()[RAI_KEY_LCTRL]) {
          objId = readObjIdx();
          focus = true;
        } else if (e.button.clicks == 1 && keyboard()[RAI_KEY_LCTRL] && readObjIdx() == highlightedObjId)
          startInteraction = isInteracting_ = true;
        else if (e.button.clicks == 1)
          objId = readObjIdx();
        break;
      case SDL_MOUSEBUTTONUP:isInteracting_ = false;
        break;
      case SDL_KEYDOWN:
        if (keyboard()[RAI_KEY_ESCAPE] && highlightedObjId != NO_OBJECT && objectsInOrder_[highlightedObjId])
          objectsInOrder_[highlightedObjId]->deHighlight();

        if (keyboard()[RAI_KEY_R] && keyboard()[RAI_KEY_LSHIFT])
          if (!saveSnapShot) {
            RAIINFO("Saving video");
            savingSnapshots_private("/tmp", "raiGraphicsAutoRecorder" + std::to_string(autoVideoRecordingNumber++));
          } else {
            RAIINFO("Starting encoding video");
            images2Video();
          }

        for (int fkey = 0; fkey < TEXTMENUCOUNT; fkey++)
          if (keyboard()[RAI_KEY_F1 + fkey])
            menuTextToggle[fkey] = !menuTextToggle[fkey];

        for (int togKey = 0; togKey < 10; togKey++)
          if (keyboard()[RAI_KEY_1 + togKey])
            customToggle[togKey] = !customToggle[togKey];

        if (keyboard()[RAI_KEY_U])
          keyboardEvent[0] = true;

        if (keyboard()[RAI_KEY_I])
          keyboardEvent[1] = true;

        if (keyboard()[RAI_KEY_O])
          keyboardEvent[2] = true;

        if (keyboard()[RAI_KEY_P])
          keyboardEvent[3] = true;

        if (keyboard()[RAI_KEY_J])
          keyboardEvent[4] = true;

        if (keyboard()[RAI_KEY_K])
          keyboardEvent[5] = true;

        if (keyboard()[RAI_KEY_L])
          keyboardEvent[6] = true;

        if (keyboard()[RAI_KEY_DELETE])
          keyboardEvent[7] = true;

        if (keyboard()[RAI_KEY_Q])
          isQuiting = true;

        break;
      case SDL_MOUSEWHEEL:
        if (keyboard()[RAI_KEY_LCTRL] && e.wheel.y == 1)
          interactionMagnitude *= 1.2;
        else if (keyboard()[RAI_KEY_LCTRL] && e.wheel.y == -1)
          interactionMagnitude /= 1.2;
        else if (keyboard()[RAI_KEY_T] && e.wheel.y == 1)
          realtimeFactor *= 1.2;
        else if (keyboard()[RAI_KEY_T] && e.wheel.y == -1)
          realtimeFactor /= 1.2;
        else if (e.wheel.y == 1)
          camera->zoomOut();
        else if (e.wheel.y == -1)
          camera->zoomIn();
        break;
    }
  }
  loopcounter++;

  for (int fkey = 0; fkey < TEXTMENUCOUNT; fkey++)
    if (menuTextToggle[fkey])
      textBoard[fkey]->writeText(font, menuText[fkey][1]);
    else
      textBoard[fkey]->writeText(font, menuText[fkey][0]);

  if (objId != NO_OBJECT && objId != 0 && objId < objectsInOrder_.size()) {
    if (focus)
      camera->follow(objectsInOrder_[objId]);
    interactingObjSelectableId = objectsInOrder_[objId]->getSelectableObIndex();

    if (highlightedObjId != NO_OBJECT && objectsInOrder_[highlightedObjId]) {
      objectsInOrder_[highlightedObjId]->deHighlight();
    }
    highlightedObjId = objId;
    objectsInOrder_[objId]->highlight();
  }

  display->Clear(1,1,1,0);

  /// update camera with events
  camera->Control(e, true);
  camera->update();

  if (startInteraction)
    SDL_GetMouseState(&interStartingX, &interStartingY);

  if (isInteracting_)
    computeMousePull();

  /// draw checkerboard and reflections
  if (checkerboard) {
    if (!checkerboard->gridMode) {
      drawObj(true);
      shader_checkerboard->Bind();
      shader_checkerboard->Update(camera, light, checkerboard);
      checkerboard->draw();
      shader_checkerboard->UnBind();
    } else {
      shader_line->Bind();
      std::vector<float> color;
      checkerboard->getColor(color);
      shader_line->Update(camera, light, color);
      checkerboard->drawGridLines();
      shader_line->UnBind();
    }
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

  /// menu
  for (int tbId = 0; tbId < TEXTMENUCOUNT; tbId++) {
    shader_menu->Bind();
    shader_menu->Update(textBoard[tbId]);
    textBoard[tbId]->bindTexture();
    textBoard[tbId]->draw();
    shader_menu->UnBind();
  }

  /// line primitives
  for(auto& ln: lines_){
    shader_line->Bind();
    shader_line->Update(camera, light, ln->color);
    ln->draw();
    shader_line->UnBind();

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
      FreeImage_Save(FIF_BMP, image, (image_dir + "/" + imageFileName + ".bmp").c_str(), 1023);
      FreeImage_Unload(image);
      delete[] pixels;
    } else {
      RAIWARN("RAI is saving video now since it exceeded the time limit");
      images2Video();
      saveSnapShot = false;
    }
  }
  display->SwapBuffers();
}

void RAI_graphics::addObject(object::SingleBodyObject *obj, object::ShaderType type) {
  std::lock_guard<std::mutex> guard(mtxinit);
  RAIFATAL_IF(!obj, "the object is not created yet");
  added_objs_.push_back(obj);
  if (type == object::RAI_SHADER_OBJECT_DEFAULT)
    type = obj->defaultShader;
  added_shaders_.push_back(type);
  obj->setSelectableObIndex(++selectableIndexToBeAssigned);
  objectsInOrder_.push_back(obj);
}

object::Lines* RAI_graphics::addLineSet() {
  lines_.push_back(new object::Lines);
  return lines_.back();
}

void RAI_graphics::removeLineSet(object::Lines* lineset) {
  for(int i=0; i<lines_.size(); i++)
    if(lines_[i]==lineset) {
      delete lines_[i];
      lines_[i] = lines_.back();
      lines_.pop_back();
    }
}

void RAI_graphics::addSuperObject(object::MultiBodyObject *obj) {
  std::lock_guard<std::mutex> guard(mtxinit);
  RAIFATAL_IF(!obj, "the object is not created yet");
  added_supObjs_.push_back(obj);
  for (auto &ob: obj->getChildren()) {
    ob->setSelectableObIndex(++selectableIndexToBeAssigned);
    objectsInOrder_.push_back(ob);
  }
}

void RAI_graphics::addBackground(object::Background *back) {
  std::lock_guard<std::mutex> guard(mtxinit);
  backgroundChanged = true;
  RAIFATAL_IF(!back, "the object is not created yet");
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

void RAI_graphics::removeAndDeleteObject(object::SingleBodyObject *obj) {
  std::lock_guard<std::mutex> guard(mtxinit);
  tobeRemovedAndDeleted_objs_.push_back(obj);
}

bool RAI_graphics::getKeyboardEvent(KeyboardEvent ke) {
  if (keyboardEvent[int(ke)]) {
    keyboardEvent[int(ke)] = false;
    return true;
  } else
    return false;
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

void RAI_graphics::setAntiAliasing(int aa) {
  RAIFATAL_IF(aa > 8 || aa < 1, "AntiAliasing should be between 1 and 8")
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, aa);
}

void RAI_graphics::setCameraProp(CameraProp &prop) {
  cameraProp = prop;
  cameraPropChanged = true;
}

void RAI_graphics::savingSnapshots(std::string logDirectory, std::string fileName) {
  mtx.lock();
  savingSnapshots_private(logDirectory, fileName);
  mtx.unlock();
}

void RAI_graphics::savingSnapshots_private(std::string logDirectory, std::string fileName) {
  image_dir = logDirectory;
  imageCounter = 0;
  videoFileName = fileName;
  saveSnapShot = true;
}

void RAI_graphics::images2Video() {
  std::lock_guard<std::mutex> guard(mtxCamera);
  saveSnapShot = false;
  if (!areThereimagesTosave) return;
  areThereimagesTosave = false;
  Thread2Ptr t = &RAI_graphics::images2Video_inThread;
  PthreadPtr p = *(PthreadPtr *) &t;
  pthread_t tid;
  if (pthread_create(&tid, 0, p, this) == 0)
    pthread_detach(tid);
}

void *RAI_graphics::images2Video_inThread(void *obj) {
  std::string
    command =
    "ffmpeg -r 60 -i " + image_dir + "/%07d.bmp -s " + std::to_string(windowWidth_) + "x" + std::to_string(windowHeight_) + " -c:v libx264 -crf 1 "
      + image_dir + "/" + videoFileName + ".mp4 >nul 2>&1";
  std::ifstream f((image_dir + "/" + videoFileName + ".mp4").c_str());
  if (f.good())
    int i = system(("rm " + image_dir + "/" + videoFileName + ".mp4").c_str());
  int i = system(command.c_str());
  command = "rm -rf " + image_dir + "/*.bmp";
  i = system(command.c_str());
  RAIINFO("The video is generated under " + image_dir);
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
    if (sob->isVisible() && isReflection)
      sob->draw(camera, light, 1.0, checkerboard);
    else if (sob->isVisible())
      sob->draw(camera, light, 1.0);

  for (int i = 0; i < objs_.size(); i++) {
    if ((!objs_[i]->isVisible() && objs_[i]->getGhosts().size() == 0) || (!objs_[i]->reflectable && isReflection)) continue;

    if (objs_[i]->isVisible()) {
      shaders_[i]->Bind();
      if (isReflection)
        shaders_[i]->UpdateForReflection(camera, light, objs_[i], checkerboard);
      else
        shaders_[i]->Update(camera, light, objs_[i]);
      objs_[i]->draw();
      shaders_[i]->UnBind();
    }

    // draw ghost
    // TODO code refine
    objs_[i]->mutexLock();
    objs_[i]->usingTempTransform(true);
    for (int j = 0; j < objs_[i]->getGhosts().size(); j++) {
      objs_[i]->setTempTransform(j);
      shaders_[i]->Bind();
      shaders_[i]->Update(camera, light, objs_[i]);
      objs_[i]->draw();
      shaders_[i]->UnBind();
    }
    objs_[i]->usingTempTransform(false);
    objs_[i]->mutexUnLock();
  }
}

void RAI_graphics::computeMousePull() {
  int tx, ty;
  Transform objTrans;
  glm::mat4 cameraT;
  glm::vec3 cameraPos, objectPos, diffPos, arrowEnd3norm, arrowEnd3;
  camera->GetPose(cameraT);
  camera->GetPos(cameraPos);

  objectsInOrder_[highlightedObjId]->getTransform(objTrans);
  glm::vec4 arrowOrigin = objTrans.GetModel() * glm::vec4(objectsInOrder_[highlightedObjId]->com, 1);
  glm::vec4 arrowOrigin_cam = cameraT * arrowOrigin;

  SDL_GetMouseState(&tx, &ty);
  tx -= windowWidth_ / 2.0;
  ty -= windowHeight_ / 2.0;

  diffPos = cameraPos - *objTrans.GetPos();
  float normDiff = glm::l2Norm(diffPos);
  glm::vec4 arrowEnd(-tx / cameraDepth_ * arrowOrigin_cam[2], ty / cameraDepth_ * arrowOrigin_cam[2], arrowOrigin_cam[2], 1);
  glm::vec4 arrowEnd4 = glm::inverse(cameraT) * arrowEnd;
  arrowEnd3 = glm::vec3(arrowEnd4 - arrowOrigin);
  arrowEnd3norm = glm::normalize(arrowEnd3);
  interactionArrow->setScale(glm::l2Norm(arrowEnd3));

  glm::vec3 axis = glm::cross(arrowEnd3norm, glm::vec3(1, 0, 0));
  double angle = acos(arrowEnd3norm[0]);
  axis = glm::normalize(axis);
  Eigen::Vector3d axisE(axis.x, axis.y, axis.z);
  Eigen::Vector3d arrowPosE(arrowOrigin[0], arrowOrigin[1], arrowOrigin[2]);
  interactionArrow->setPos(arrowPosE);
  Eigen::Vector4d quat = rai::Math::MathFunc::angleAxisToQuat(-angle, axisE);
  interactionArrow->setOri(quat);
  shader_basic->Bind();
  shader_basic->Update(camera, light, interactionArrow);
  interactionArrow->draw();
  shader_basic->UnBind();
  interactionForce << arrowEnd3.x, arrowEnd3.y, arrowEnd3.z;
  if (interactionForce.norm() < 1e-5)
    interactionForce.setZero();
  interactionForce /= arrowOrigin_cam[2] / -interactionMagnitude;
}

bool RAI_graphics::isInteracting() {
  return isInteracting_;
}

bool RAI_graphics::getCustomToggleState(int id) {
  RAIFATAL_IF(id < 0 || id > 10, "Available toggle keys: 0~10")
  return (id == 0) ? customToggle[9] : customToggle[id - 1];
}

Eigen::Vector3d &RAI_graphics::getInteractionMagnitude() {
  return interactionForce;
}

float RAI_graphics::getRealTimeFactor() {
  return realtimeFactor;
}

int RAI_graphics::getInteractingObjectID() {
  return interactingObjSelectableId;
}

void RAI_graphics::changeMenuText(int menuId, bool isOnText, std::string mt) {
  menuText[menuId][isOnText] = mt;
}

void RAI_graphics::changeMenuPosition(int menuId, int x, int y) {
  textBoard[menuId]->setTranslation(x, y);
}

void RAI_graphics::setMenuPositionNextToCursor(int menuId) {
  int x, y;
  SDL_GetMouseState(&x, &y);
  textBoard[menuId]->setTranslation(x + 5, windowHeight_ - y - 5);
}

void RAI_graphics::changeMenuWordWrap(int menuId, int wr) {
  textBoard[menuId]->setTextWrap(wr);
}

void RAI_graphics::changeMenuFontSize(int menuId, int size) {
  textBoard[menuId]->setFontSize(size);
}

} // rai_graphics
