//
// Created by kangd on 28.09.17.
//

#include "raiGraphics/obj/Frame.hpp"

namespace RAI {
namespace Graphics {
namespace Obj {

Frame::Frame(Eigen::Vector3d origin,
             float arrowBodyLength,
             float arrowHeadLength,
             float arrowBodyRadius,
             float arrowHeadRadius,
             bool colorYn)
    : origin_(origin),
      arrowBodyLength_(arrowBodyLength),
      arrowHeadLength_(arrowHeadLength),
      arrowBodyRadius_(arrowBodyRadius),
      arrowHeadRadius_(arrowHeadRadius),
      xAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      yAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      zAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength) {

  Frame::xAxisArrowRotMat_ <<   1,  0,  0,
      0,  1,  0,
      0,  0,  1;

  Frame::yAxisArrowRotMat_ <<   0, -1,  0,
      1,  0,  0,
      0,  0,  1;

  Frame::zAxisArrowRotMat_ <<   0,  0,  -1,
      0,  1,  0,
      1,  0,  0;

  if (colorYn) {
    xAxisArrow_.setColor({1.0, 0.0, 0.0});   // X = R
    yAxisArrow_.setColor({0.0, 1.0, 0.0});   // Y = G
    zAxisArrow_.setColor({0.0, 0.0, 1.0});   // Z = B
  }

  xAxisArrow_.setOri(xAxisArrowRotMat_);
  yAxisArrow_.setOri(yAxisArrowRotMat_);
  zAxisArrow_.setOri(zAxisArrowRotMat_);

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);

  objs.push_back(&xAxisArrow_);
  objs.push_back(&yAxisArrow_);
  objs.push_back(&zAxisArrow_);

}

void Frame::setPos(Eigen::Vector3d &position) {
  origin_ = position;

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);
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