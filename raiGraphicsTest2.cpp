//
// Created by jhwangbo on 17. 4. 28.
//

#include "raiGraphics/RAI_graphics.hpp"

using namespace rai_graphics;

int main() {

  RAI_graphics graphics(800,600);
  std::string urdfPath(__FILE__);
  while (urdfPath.back() != '/')
    urdfPath.erase(urdfPath.size() - 1, 1);
  urdfPath += "/res/ANYmal/anymal_base.dae";

  // graphical primitives
  object::CheckerBoard grnd(5, 500, 500, 0.05, {0,0,0}, {1,1,1});
  object::Mesh body(urdfPath, 0.001, "");
  LightProp lprop;
  lprop.pos_light = {10.0, 0.0, 10.0};
  CameraProp cprop;
  cprop.toFollow = &body;
  Eigen::Vector3d relPos; relPos << 15, 15, 15;
  cprop.relativeDist = relPos;

  graphics.addCheckerBoard(&grnd);
  graphics.addObject(&body);

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