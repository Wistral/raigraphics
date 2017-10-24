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

  // graphical primitives
  object::Sphere whiteSphere(1);
  object::Cone redCone(1, 2);
  object::Box greenBox(2, 2, 2);
  object::Cylinder blueCylinder(1, 2);
  object::Background background("sky");
  object::CoordinateFrame inertiaFrame;

  Eigen::Vector3d bodyFrameOrigin1(10.0, 0.0, 0.0);
  Eigen::Quaterniond bodyFrameQuat1(0.9239, 0.3827, 0, 0);
  object::CoordinateFrame bodyFrame1(bodyFrameOrigin1, bodyFrameQuat1);
  Eigen::Vector3d bodyFrameOrigin2(0.0, 10.0, 0.0);
  Eigen::Quaterniond bodyFrameQuat2(0.9239, 0, 0.3827, 0);
  object::CoordinateFrame bodyFrame2(bodyFrameOrigin2, bodyFrameQuat2);
  Eigen::Vector3d bodyFrameOrigin3(0.0, 0.0, 10.0);
  Eigen::Quaterniond bodyFrameQuat3(0.9239, 0, 0, 0.3827);
  object::CoordinateFrame bodyFrame3(bodyFrameOrigin3, bodyFrameQuat3);

  whiteSphere.setColor({1.0, 1.0, 1.0});
  redCone.setColor({1.0, 0.0, 0.0});
  greenBox.setColor({0.0, 1.0, 0.0});
  blueCylinder.setColor({0.0, 0.0, 1.0});

  Eigen::Vector3d whiteSpherePosition; whiteSpherePosition << 0, 0, 0;
  Eigen::Vector3d redConePosition; redConePosition << 10, 0, 0;
  Eigen::Quaterniond redConeQuaternion(0.9239, 0.3827, 0, 0);
  Eigen::Vector3d greenBoxPosition; greenBoxPosition << 0, 10, 0;
  Eigen::Quaterniond greenBoxQuaternion(0.9239, 0, 0.3827, 0);
  Eigen::Vector3d blueCylinderPosition; blueCylinderPosition << 0, 0, 10;
  Eigen::Quaterniond blueCylinderQuaternion(0.9239, 0, 0, 0.3827);

  whiteSphere.setPos(whiteSpherePosition);
  redCone.setPose(redConePosition, redConeQuaternion);
  greenBox.setPose(greenBoxPosition, greenBoxQuaternion);
  blueCylinder.setPose(blueCylinderPosition, blueCylinderQuaternion);

  object::Arrow directionArrow(0.1, 0.2, 4, 0.2);
  directionArrow.setColor({1.0, 1.0, 0.0});
  Eigen::Vector3d directionVector(0.0, 0.0, -1.0);
  directionArrow.representVector(directionVector);

  graphics.addBackground(&background);
  graphics.addObject(&whiteSphere);
  graphics.addObject(&redCone);
  graphics.addObject(&greenBox);
  graphics.addObject(&blueCylinder);
  graphics.addSuperObject(&inertiaFrame);
  graphics.addSuperObject(&bodyFrame1);
  graphics.addSuperObject(&bodyFrame2);
  graphics.addSuperObject(&bodyFrame3);
  graphics.addObject(&directionArrow);

  // ghost objects
  Eigen::Vector3d ghostVector1; ghostVector1 << 10, 0, 0;
  Eigen::Quaterniond ghostQuaternion1(1.0, 0, 0, 0);
  directionArrow.addGhost(ghostVector1, ghostQuaternion1);

  Eigen::Vector3d ghostVector2; ghostVector2 << 0, 10, 0;
  Eigen::Quaterniond ghostQuaternion2(0.7071, 0, 0, 0.7071);
  directionArrow.addGhost(ghostVector2, ghostQuaternion1);

  RAI_graphics::LightProp lprop;
  RAI_graphics::CameraProp cprop;
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