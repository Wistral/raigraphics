//
// Created by kangd on 28.09.17.
//

#include <raiGraphics/imp/shader_flat.h>
#include "raiGraphics/obj/CoordinateFrame.hpp"

namespace rai_graphics {
namespace object {

CoordinateFrame::CoordinateFrame(Eigen::Vector3d origin,
                                 float arrowBodyLength,
                                 float arrowHeadLength,
                                 float arrowBodyRadius,
                                 float arrowHeadRadius)
    : origin_(origin),
      rotation_({1, 0, 0, 0}),
      arrowBodyLength_(arrowBodyLength),
      arrowHeadLength_(arrowHeadLength),
      arrowBodyRadius_(arrowBodyRadius),
      arrowHeadRadius_(arrowHeadRadius),
      xAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      yAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      zAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength) {
  initChildren();
}

CoordinateFrame::CoordinateFrame(Eigen::Vector3d &origin,
                                 Eigen::Quaterniond &rotation,
                                 float arrowBodyLength,
                                 float arrowHeadLength,
                                 float arrowBodyRadius,
                                 float arrowHeadRadius)
    : origin_(origin),
      rotation_(rotation),
      arrowBodyLength_(arrowBodyLength),
      arrowHeadLength_(arrowHeadLength),
      arrowBodyRadius_(arrowBodyRadius),
      arrowHeadRadius_(arrowHeadRadius),
      xAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      yAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength),
      zAxisArrow_(arrowBodyRadius, arrowHeadRadius, arrowBodyLength, arrowHeadLength) {
  initChildren();
}

void CoordinateFrame::initChildren() {
  WxAxisArrowQuaternion_ = rotation_ * BxAxisArrowQuaternion_;
  WyAxisArrowQuaternion_ = rotation_ * ByAxisArrowQuaternion_;
  WzAxisArrowQuaternion_ = rotation_ * BzAxisArrowQuaternion_;

  xAxisArrow_.setOri(WxAxisArrowQuaternion_);
  yAxisArrow_.setOri(WyAxisArrowQuaternion_);
  zAxisArrow_.setOri(WzAxisArrowQuaternion_);

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);

  xAxisArrow_.setColor({1.0, 0.0, 0.0});   // X = R
  yAxisArrow_.setColor({0.0, 1.0, 0.0});   // Y = G
  zAxisArrow_.setColor({0.0, 0.0, 1.0});   // Z = B

  objs.push_back(&xAxisArrow_);
  objs.push_back(&yAxisArrow_);
  objs.push_back(&zAxisArrow_);
  reflectable = false;
}

void CoordinateFrame::setPos(const Eigen::Vector3d &position) {
  origin_ = position;

  xAxisArrow_.setPos(origin_);
  yAxisArrow_.setPos(origin_);
  zAxisArrow_.setPos(origin_);
}

void CoordinateFrame::setPos(double x, double y, double z) {
  origin_<<x,y,z;
  xAxisArrow_.setPos(x,y,z);
  yAxisArrow_.setPos(x,y,z);
  zAxisArrow_.setPos(x,y,z);
}

void CoordinateFrame::init() {
  for(auto* ob: objs)
    ob->init();
  for(auto* ob: objs)
    shader.push_back(new Shader_basic);
}

void CoordinateFrame::destroy() {
  for(auto* ob: objs)
    ob->destroy();
  for(auto* sh: shader)
    delete sh;
}

CoordinateFrame::~CoordinateFrame() {
}

void CoordinateFrame::setOri(const Eigen::Quaterniond &quaternionWB) {
  rotation_ = quaternionWB;
  WxAxisArrowQuaternion_ = quaternionWB * BxAxisArrowQuaternion_;
  WyAxisArrowQuaternion_ = quaternionWB * ByAxisArrowQuaternion_;
  WzAxisArrowQuaternion_ = quaternionWB * BzAxisArrowQuaternion_;

  xAxisArrow_.setOri(WxAxisArrowQuaternion_);
  yAxisArrow_.setOri(WyAxisArrowQuaternion_);
  zAxisArrow_.setOri(WzAxisArrowQuaternion_);
}
void CoordinateFrame::setOri(const Eigen::Vector4d &quaternionAsVectorWB) {
  Eigen::Quaterniond quaternion(quaternionAsVectorWB(0),
                                quaternionAsVectorWB(1),
                                quaternionAsVectorWB(2),
                                quaternionAsVectorWB(3));
  setOri(quaternion);
}
void CoordinateFrame::setOri(const Eigen::Matrix3d &rotationMatrixWB) {
  Eigen::Quaterniond quaternion(rotationMatrixWB);
  setOri(quaternion);
}
void CoordinateFrame::setPose(const Eigen::Vector3d &position, const Eigen::Matrix3d &rotationMatrixWB) {
  setPos(position);
  setOri(rotationMatrixWB);
}
void CoordinateFrame::setPose(const Eigen::Vector3d &position, const Eigen::Vector4d &quaternionAsVectorWB) {
  setPos(position);
  setOri(quaternionAsVectorWB);
}
void CoordinateFrame::setPose(const Eigen::Vector3d &position, const Eigen::Quaterniond &quaternionWB) {
  setPos(position);
  setOri(quaternionWB);
}

} // object
} // rai_graphics