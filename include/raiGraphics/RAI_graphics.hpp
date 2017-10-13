//
// Created by jhwangbo on 17. 4. 28.
//

#ifndef PROJECT_RAI_GRAPHICS_HPP
#define PROJECT_RAI_GRAPHICS_HPP
#include "raiGraphics/RAI_keyboard.hpp"
#include "raiGraphics/obj/Background.hpp"
#include "raiGraphics/obj/Object.hpp"
#include "raiGraphics/obj/SuperObject.hpp"
#include "raiGraphics/imp/display.h"
#include "raiGraphics/imp/shader_basic.h"
#include "raiGraphics/imp/shader_background.hpp"
#include "rai/common/StopWatch.hpp"
#include <mutex>

namespace rai_graphics {

class RAI_graphics {
 public:

  enum ShaderType {
    RAI_SHADER_BASIC
  };

  struct LightProp {
    std::vector<float>  pos_light = {-100.0,0.0,10.0},
                        amb_light = {0.5, 0.5, 0.5},
                        diff_light = {1, 1, 1},
                        spec_light = {0.7, 0.7, 0.7};
  };

  struct MouseInput {
    int x, y;
    bool leftB, rightB, middleB;
    SDL_MouseWheelEvent wheel;
  };

  struct CameraProp {
    object::Object *toFollow = nullptr;
    Eigen::Vector3d relativeDist = Eigen::Vector3d::Constant(1);
  };

  RAI_graphics(int windowWidth, int windowHeight);
  ~RAI_graphics();
  void start();
  void end();

  void addObject(object::Object *obj, ShaderType type = RAI_SHADER_BASIC);
  void addSuperObject(object::SuperObject *obj);
  void addBackground(object::Background *back);
  void setFPS(double FPS) { FPS_ = FPS; }

  void removeObject(object::Object *obj);
  void removeSuperObject(object::SuperObject *obj);
  void setBackgroundColor(float r, float g, float b, float a);
  void setLightProp(LightProp &prop);
  void setCameraProp(CameraProp &prop);

  void savingSnapshots(std::string logDirectory, std::string fileName);
  void images2Video();

  const Uint8* keyboard();
  const MouseInput* mouse();

 private:
  void *loop(void *obj);
  void init();
  void draw();
  void *images2Video_inThread(void *obj);

  object::Background *background = nullptr;
  bool backgroundChanged;

  std::vector<object::Object *> objs_;
  std::vector<object::SuperObject *> supObjs_;

  std::vector<object::Object *> added_objs_;
  std::vector<object::SuperObject *> added_supObjs_;

  std::vector<object::Object *> tobeRemoved_objs_;
  std::vector<object::SuperObject *> tobeRemoved_supObjs_;

  Display *display = nullptr;
  Shader_basic *shader_basic = nullptr;
  Shader_background *shader_background = nullptr;
  std::vector<Shader *> shaders_;
  std::vector<ShaderType> added_shaders_;

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

};

} // rai_graphics

#endif //PROJECT_RAI_GRAPHICS_HPP
