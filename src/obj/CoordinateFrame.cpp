//
// Created by kangd on 28.09.17.
//

#include "raiGraphics/obj/CoordinateFrame.hpp"

namespace rai_graphics {
namespace objects {

CoordinateFrame::CoordinateFrame(Eigen::Vector3d origin,
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

  Eigen::Matrix3d xAxisArrowRotMat;
  xAxisArrowRotMat <<   1,  0,  0,
      0,  1,  0,
      0,  0,  1;

  Eigen::Matrix3d yAxisArrowRotMat;
  yAxisArrowRotMat <<   0, -1,  0,
      1,  0,  0,
      0,  0,  1;

  Eigen::Matrix3d zAxisArrowRotMat;
  zAxisArrowRotMat <<   0,  0,  -1,
      0,  1,  0,
      1,  0,  0;

  Eigen::Quaternion<double> xAxisArrowQuat = Eigen::Quaternion<double>(xAxisArrowRotMat);
  Eigen::Quaternion<double> yAxisArrowQuat = Eigen::Quaternion<double>(yAxisArrowRotMat);
  Eigen::Quaternion<double> zAxisArrowQuat = Eigen::Quaternion<double>(zAxisArrowRotMat);

  if (colorYn) {
    xAxisArrow_.setColor({1.0, 0.0, 0.0});   // X = R
    yAxisArrow_.setColor({0.0, 1.0, 0.0});   // Y = G
    zAxisArrow_.setColor({0.0, 0.0, 1.0});   // Z = B
  }

  xAxisArrowRot_ << xAxisArrowQuat.w(),
      xAxisArrowQuat.x(),
      xAxisArrowQuat.y(),
      xAxisArrowQuat.z();
  yAxisArrowRot_ << yAxisArrowQuat.w(),
      yAxisArrowQuat.x(),
      yAxisArrowQuat.y(),
      yAxisArrowQuat.z();
  zAxisArrowRot_ << xAxisArrowQuat.w(),
      zAxisArrowQuat.x(),
      zAxisArrowQuat.y(),
      zAxisArrowQuat.z();

  xAxisArrow_.setOri(xAxisArrowRot_);
  yAxisArrow_.setOri(yAxisArrowRot_);
  zAxisArrow_.setOri(zAxisArrowRot_);

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);

  objs.push_back(&xAxisArrow_);
  objs.push_back(&yAxisArrow_);
  objs.push_back(&zAxisArrow_);

}

void CoordinateFrame::setPos(Eigen::Vector3d &position) {
  origin_ = position;

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);
}

void CoordinateFrame::init() {
  for(auto* ob: objs)
    ob->init();
  shader = new Shader_basic;
}

void CoordinateFrame::destroy() {
  for(auto* ob: objs)
    ob->destroy();
  delete shader;
}

CoordinateFrame::~CoordinateFrame() {
}

void CoordinateFrame::setOri(Eigen::Vector4d &quat) {
//  xAxisArrow_.setPos(quat * xAxisArrowRot_);
//  yAxisArrow_.setPos(quat * yAxisArrowRot_);
//  zAxisArrow_.setPos(quat * zAxisArrowRot_);
}
void CoordinateFrame::setOri(Eigen::Matrix3d &rotationMat) {
  Eigen::Quaternion<double> quatFromRotMat = Eigen::Quaternion<double>(rotationMat);
  Eigen::Vector4d quat;
  quat << quatFromRotMat.w(),
      quatFromRotMat.x(),
      quatFromRotMat.y(),
      quatFromRotMat.z();
  setOri(quat);
}
void CoordinateFrame::setPose(Eigen::Vector3d &position, Eigen::Vector4d &quat) {
  setPos(position);
  setOri(quat);
}
void CoordinateFrame::setPose(Eigen::Vector3d &position, Eigen::Matrix3d &rotationMat) {
  setPos(position);
  setOri(rotationMat);
}

} // objects
} // rai_graphics