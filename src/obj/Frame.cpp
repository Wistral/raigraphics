//
// Created by kangd on 28.09.17.
//

#include "raiGraphics/obj/Frame.hpp"

namespace RAI {
namespace Graphics {
namespace Obj {

Frame::Frame(bool colorYn):
    xAxisArrow(0.5, 1, 2, 2),
    yAxisArrow(0.5, 1, 2, 2),
    zAxisArrow(0.5, 1, 2, 2)
{

  Eigen::Vector3d origin; origin << 0, 0, 0;

  Eigen::Matrix3d xAxisArrowRotMat;
  Eigen::Matrix3d yAxisArrowRotMat;
  Eigen::Matrix3d zAxisArrowRotMat;

  xAxisArrowRotMat <<   1,  0,  0,
                        0,  1,  0,
                        0,  0,  1;

  yAxisArrowRotMat <<   0, -1,  0,
                        1,  0,  0,
                        0,  0,  1;

  zAxisArrowRotMat <<   0,  0,  -1,
                        0,  1,  0,
                        1,  0,  0;

  if (colorYn) {
    xAxisArrow.setColor({1.0, 0.0, 0.0});   // X = R
    yAxisArrow.setColor({0.0, 1.0, 0.0});   // Y = G
    zAxisArrow.setColor({0.0, 0.0, 1.0});   // Z = B
  }

  xAxisArrow.setOri(xAxisArrowRotMat);
  yAxisArrow.setOri(yAxisArrowRotMat);
  zAxisArrow.setOri(zAxisArrowRotMat);

  xAxisArrow.setPos(origin);
  yAxisArrow.setPos(origin);
  zAxisArrow.setPos(origin);

  objs.push_back(&xAxisArrow);
  objs.push_back(&yAxisArrow);
  objs.push_back(&zAxisArrow);

}

void Frame::init() {
  for(auto* ob: objs)
    ob->init();
  shader = new Shader_basic;
}

void Frame::destroy() {
  for(auto* ob: objs)
    ob->destroy();
  delete shader;
}

Frame::~Frame() {
}


} // Obj
} // Graphics
} // RAI