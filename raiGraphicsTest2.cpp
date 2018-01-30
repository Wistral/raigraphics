//
// Created by jhwangbo on 17. 4. 28.
//

#include <raiGraphics/obj/Capsule.hpp>
#include "raiGraphics/RAI_graphics.hpp"
#include "raiGraphics/obj/CoordinateFrame.hpp"
//#include <raiGraphics/obj/Mesh.hpp>
#include "raiGraphics/obj/Sphere.hpp"
#include "raiGraphics/obj/Cone.hpp"
#include "raiGraphics/obj/Box.hpp"
#include "raiGraphics/obj/Cylinder.hpp"

using namespace rai_graphics;

int main() {

  RAI_graphics graphics(800,600);

  // graphical primitives
  object::Sphere whiteSphere(1, true);
  whiteSphere.setPos(0,0,0);
  object::CheckerBoard grnd(5, 500, 500, 0.05, {0,0,0}, {1,1,1});

  LightProp lprop;
  lprop.pos_light = {10.0, 0.0, 10.0};
  CameraProp cprop;
  cprop.toFollow = &whiteSphere;
  Eigen::Vector3d relPos; relPos << 15, 15, 15;
  cprop.relativeDist = relPos;
  graphics.addObject(&whiteSphere);
  graphics.addCheckerBoard(&grnd);

  auto* line = graphics.addLineSet();

  line->addSegment(0,0,0,0,0,3);
  line->setColor(0,0,0);
  line->setLineWidth(3.0);
  grnd.setBoardColor({1,0,0}, {1,1,1});

  graphics.setCameraProp(cprop);
  graphics.setLightProp(lprop);
  graphics.start();

  // exit test app when keyboard 'Q' key pressed
  while(!graphics.isQuitting())
    usleep(1);

  graphics.end();

  return 0;
}