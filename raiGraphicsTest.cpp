//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"
#include <raiGraphics/obj/Frame.hpp>
#include <raiGraphics/obj/Mesh.hpp>
#include <raiGraphics/obj/Sphere.hpp>

using namespace RAI::Graphics;

int main() {

  // TODO: RAI_ROOT in every file should be changed
  RAI_graphics graphics(800, 600);

//  TODO anymal will be removed from raiGraphics
//  Obj::Mesh anymalBase(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT"))+"/res/anymal_base_1_2.dae", 0.001);
//  anymalBase.setScale(5.0);
//  anymalBase.setTransparency(1.0);

  Obj::Mesh terrain(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT"))+"/res/roughterrain.obj");
  Obj::Sphere whiteSphere(1);
  Obj::Sphere redSphere(1);
  Obj::Sphere greenSphere(1);
  Obj::Sphere blueSphere(1);
  Obj::Background background("sky");
  Obj::Frame inertiaFrame(true);

  whiteSphere.setColor({1.0, 1.0, 1.0});
  redSphere.setColor({1.0, 0.0, 0.0});
  greenSphere.setColor({0.0, 1.0, 0.0});
  blueSphere.setColor({0.0, 0.0, 1.0});
  terrain.setColor({0.7, 0.2, 0.2});

  Eigen::Vector3d whiteSpherePosition; whiteSpherePosition << 0, 0, 0;
  Eigen::Vector3d redSpherePosition; redSpherePosition << 5, 0, 0;
  Eigen::Vector3d blueSpherePosition; blueSpherePosition << 0, 5, 0;
  Eigen::Vector3d greenSpherePosition; greenSpherePosition << 0, 0, 5;

//  whiteSphere.setPos(whiteSpherePosition);
  redSphere.setPos(redSpherePosition);
  greenSphere.setPos(blueSpherePosition);
  blueSphere.setPos(greenSpherePosition);
  graphics.addBackground(&background);
//  graphics.addObject(&terrain);
//  graphics.addObject(&anymalBase);
  graphics.addObject(&whiteSphere);
  graphics.addObject(&redSphere);
  graphics.addObject(&greenSphere);
  graphics.addObject(&blueSphere);
  graphics.addSuperObject(&inertiaFrame);

  RAI_graphics::LightProp lprop;
  RAI_graphics::CameraProp cprop;
//  cprop.toFollow = &anymalBase;
  cprop.toFollow = &whiteSphere;
  Eigen::Vector3d relPos; relPos << 20, 0, 0;
  cprop.relativeDist = relPos;

  graphics.setCameraProp(cprop);
  graphics.setLightProp(lprop);
  graphics.start();

  // exit test app when keyboard 'Q' key pressed
  while(1)
  {
    const Uint8* keys = graphics.keyboard();

    if (keys[RAI_KEY_Q]) {
      break;
    }
  }

  graphics.end();

  return 0;
}