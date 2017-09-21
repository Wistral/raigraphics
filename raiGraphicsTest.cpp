//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"
#include "raiGraphics/obj/Mesh.hpp"
#include "raiGraphics/obj/Sphere.hpp"

using namespace RAI::Graphics;

int main() {

  RAI_graphics graphics(800, 600);

  Obj::Mesh anymalBase(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT"))+"/res/anymal_base_1_2.dae", 0.001);
  anymalBase.setScale(5.0);
  anymalBase.setTransparency(1.0);

  Obj::Mesh terrain(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT"))+"/res/roughterrain.obj");
  Obj::Sphere sphere(1);
  Obj::Background background("sky");

  terrain.setColor({0.7f,0.2f,0.2f});

  Eigen::Vector3d pos; pos << 6, 0, 5;
  sphere.setPos(pos);
  graphics.addBackground(&background);
  graphics.addObject(&terrain);
  graphics.addObject(&anymalBase);
  graphics.addObject(&sphere);

  RAI_graphics::LightProp lprop;
  RAI_graphics::CameraProp cprop;
  cprop.toFollow = &anymalBase;
  Eigen::Vector3d relPos; relPos << 3, 0, 0.1;
  cprop.relativeDist = relPos;

  graphics.setCameraProp(cprop);
  graphics.setLightProp(lprop);
  graphics.start();

  while(1)
  {

  }
  graphics.end();

  return 0;
}