//
// Created by kangd on 28.09.17.
//

#ifndef RAIGRAPHICSOPENGL_FRAME_HPP
#define RAIGRAPHICSOPENGL_FRAME_HPP

#include "SuperObject.hpp"
#include "Arrow.hpp"
#include "rai/common/math/RAI_math.hpp"
#include <vector>

namespace RAI {
namespace rai_graphics {
namespace objects {

class CoordinateFrame : public SuperObject {

 public:
  explicit CoordinateFrame(Eigen::Vector3d origin = Eigen::Vector3d(0.0, 0.0, 0.0),
        float arrowBodyLength = 2.0,
        float arrowHeadLength = 1.0,
        float arrowBodyRadius = 0.25,
        float arrowHeadRadius = 0.5,
        bool colorYn = true);

  void setPose(Eigen::Vector3d &position, Eigen::Vector4d &quat);
  void setPose(Eigen::Vector3d &position, Eigen::Matrix3d &rotationMat);
  void setPos(Eigen::Vector3d &position);
  void setOri(Eigen::Vector4d &quat);
  void setOri(Eigen::Matrix3d &quat);

  virtual ~CoordinateFrame();
  void init();
  void destroy();

 private:
  Eigen::Vector3d origin_;
  Eigen::Vector4d rotation_;

//  Eigen::Matrix3d xAxisArrowRotMat_;
//  Eigen::Matrix3d yAxisArrowRotMat_;
//  Eigen::Matrix3d zAxisArrowRotMat_;

  Eigen::Vector4d xAxisArrowRot_;
  Eigen::Vector4d yAxisArrowRot_;
  Eigen::Vector4d zAxisArrowRot_;

  objects::Arrow xAxisArrow_;
  objects::Arrow yAxisArrow_;
  objects::Arrow zAxisArrow_;

  float arrowBodyLength_;
  float arrowHeadLength_;
  float arrowBodyRadius_;
  float arrowHeadRadius_;
};
} // objects
} // rai_graphics
} // RAI

#endif //RAIGRAPHICSOPENGL_FRAME_HPP
