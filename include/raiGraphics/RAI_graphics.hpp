//
// Created by jhwangbo on 17. 4. 28.
//

#ifndef PROJECT_RAI_GRAPHICS_HPP
#define PROJECT_RAI_GRAPHICS_HPP
#include "raiGraphics/RAI_keyboard.hpp"
#include "raiGraphics/obj/Background.hpp"
#include "raiGraphics/obj/SingleBodyObject.hpp"
#include "raiGraphics/obj/MultiBodyObject.hpp"
#include "raiGraphics/obj/Sphere.hpp"
#include "raiGraphics/obj/Rectangle.hpp"

#include "raiGraphics/imp/display.h"
#include "raiGraphics/imp/shader_basic.h"
#include "raiGraphics/imp/shader_flat.h"
#include "raiGraphics/imp/shader_mouseClick.h"
#include "raiGraphics/imp/shader_background.hpp"
#include "raiGraphics/imp/shader_menu.h"
#include "raiCommon/utils/StopWatch.hpp"
#include <mutex>
#include <raiGraphics/obj/CheckerBoard.hpp>
#include <raiGraphics/imp/shader_mouseClick.h>
#include <raiGraphics/obj/Arrow.hpp>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL.h"

namespace rai_graphics {

struct LightProp {
  std::vector<float>  pos_light = {-1000.0,0.0,100.0},
      amb_light = {0.2, 0.2, 0.2},
      diff_light = {0.7, 0.7, 0.7},
      spec_light = {0.7, 0.7, 0.7};
};

struct MouseInput {
  int x, y;
  bool leftB, rightB, middleB;
  SDL_MouseWheelEvent wheel;
};

struct CameraProp {
  object::SingleBodyObject *toFollow = nullptr;
  Eigen::Vector3d relativeDist = Eigen::Vector3d::Constant(1);
};

class RAI_graphics {
 public:

  RAI_graphics(int windowWidth, int windowHeight);
  ~RAI_graphics();
  void start();
  void end();

  void addObject(object::SingleBodyObject *obj, object::ShaderType type = object::RAI_SHADER_OBJECT_DEFAULT);
  void addSuperObject(object::MultiBodyObject *obj);
  void addBackground(object::Background *back);
  void addCheckerBoard(object::CheckerBoard *back);

  void setFPS(double FPS) { FPS_ = FPS; }

  void removeObject(object::SingleBodyObject *obj);
  void removeSuperObject(object::MultiBodyObject *obj);
  void setBackgroundColor(float r, float g, float b, float a);
  void setLightProp(LightProp &prop);
  void setCameraProp(CameraProp &prop);

  void savingSnapshots(std::string logDirectory, std::string fileName);
  void images2Video();

  const Uint8* keyboard();
  const MouseInput* mouse();

  bool isInteracting();
  Eigen::Vector3d& getInteractionMagnitude();
  int getInteractingObjectID();
  void drawText(const char* msg, int x, int y, int r, int g, int b);


 private:
  void *loop(void *obj);
  void init();
  void draw();
  void *images2Video_inThread(void *obj);
  int readObjIdx();
  void drawObj(bool isReflection);
  void computeMousePull();
  void savingSnapshots_private(std::string logDirectory, std::string fileName);

  object::Background *background = nullptr;
  object::CheckerBoard *checkerboard = nullptr;
  object::Arrow *interactionArrow = nullptr;
  object::Sphere *interactionBall = nullptr;
  object::Rectangle *menuBackboard = nullptr;

  bool backgroundChanged = false, checkerboardChanged = false;

  std::vector<object::SingleBodyObject *> objs_;
  std::vector<object::MultiBodyObject *> supObjs_;
  std::vector<object::SingleBodyObject *> added_objs_;
  std::vector<object::MultiBodyObject *> added_supObjs_;
  std::vector<object::SingleBodyObject *> tobeRemoved_objs_;
  std::vector<object::MultiBodyObject *> tobeRemoved_supObjs_;

  Display *display = nullptr;
  Shader_basic *shader_basic = nullptr;
  Shader_flat *shader_flat = nullptr;
  Shader_background *shader_background = nullptr;
  Shader_mouseClick *shader_mouseClick = nullptr;
  Shader_menu *shader_menu = nullptr;

  std::vector<Shader *> shaders_;
  std::vector<object::ShaderType> added_shaders_;
  std::vector<object::SingleBodyObject *> objectsInOrder_;

  unsigned imageCounter = 0;
  bool areThereimagesTosave = false;
  bool saveSnapShot = false;
  Camera *camera = nullptr;
  Light *light = nullptr;
  int windowWidth_, windowHeight_;
  SDL_Event e;
  bool freeCamMode;
  float clearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  double FPS_ = 60.0;
  std::string image_dir, videoFileName;
  StopWatch watch;
  bool terminate = false;
  std::mutex mtx;           // mutex for critical section
  std::mutex mtxLoop;           // mutex for critical section
  std::mutex mtxinit;           // mutex for critical section
  std::mutex mtxLight;
  std::mutex mtxCamera;

  pthread_t mainloopThread;
  MouseInput mouseInput;
  LightProp lightProp;
  CameraProp cameraProp;
  bool cameraPropChanged;
  bool lightPropChanged;
  unsigned objectIdexToBeAssigned = 0;
  int highlightedObjId = 16646655;
  bool isInteracting_ = false;
  int interStartingX, interStartingY;
  Eigen::Vector3d interactionForce;
  int autoVideoRecordingNumber=0;
  TTF_Font *font;

};

} // rai_graphics

#endif //PROJECT_RAI_GRAPHICS_HPP
