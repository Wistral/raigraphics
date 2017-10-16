//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"
#include "raiGraphics/obj/CoordinateFrame.hpp"
//#include <raiGraphics/obj/Mesh.hpp>
#include "raiGraphics/obj/Sphere.hpp"
#include "raiGraphics/obj/Cone.hpp"
#include "raiGraphics/obj/Box.hpp"
#include "raiGraphics/obj/Cylinder.hpp"

using namespace rai_graphics;

int main() {

  // TODO: RAI_ROOT in every file should be changed (now only task module left)
  RAI_graphics graphics(800, 600);

//  object::Mesh terrain(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT"))+"/res/roughterrain.obj");

  // graphical primitives
  object::Sphere whiteSphere(1);
  object::Cone redCone(1, 2);
  object::Box greenBox(2, 2, 2);
  object::Cylinder blueCylinder(1, 2);
  object::Background background("sky");
  object::CoordinateFrame inertiaFrame;

  whiteSphere.setColor({1.0, 1.0, 1.0});
  redCone.setColor({1.0, 0.0, 0.0});
  greenBox.setColor({0.0, 1.0, 0.0});
  blueCylinder.setColor({0.0, 0.0, 1.0});
//  terrain.setColor({0.7, 0.2, 0.2});

  Eigen::Vector3d whiteSpherePosition; whiteSpherePosition << 0, 0, 0;
  Eigen::Vector3d redConePosition; redConePosition << 5, 0, -1;
  Eigen::Vector3d greenBoxPosition; greenBoxPosition << 0, 5, 0;
  Eigen::Vector3d blueCylinderPosition; blueCylinderPosition << 0, 0, 5;

  whiteSphere.setPos(whiteSpherePosition);
  redCone.setPos(redConePosition);
  blueCylinder.setPos(blueCylinderPosition);
  greenBox.setPos(greenBoxPosition);

  graphics.addBackground(&background);
  graphics.addObject(&whiteSphere);
  graphics.addObject(&redCone);
  graphics.addObject(&greenBox);
  graphics.addObject(&blueCylinder);
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