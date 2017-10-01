//
// Created by kangd on 28.09.17.
//

#ifndef RAIGRAPHICSOPENGL_FRAME_HPP
#define RAIGRAPHICSOPENGL_FRAME_HPP

#include "SuperObject.hpp"
#include "Arrow.hpp"
#include <vector>

namespace RAI {
namespace Graphics {
namespace Obj {

class Frame : public SuperObject {

 public:
  explicit Frame(Eigen::Vector3d origin = Eigen::Vector3d(0.0, 0.0, 0.0),
        float arrowBodyLength = 2.0,
        float arrowHeadLength = 1.0,
        float arrowBodyRadius = 0.25,
        float arrowHeadRadius = 0.5,
        bool colorYn = true);

  void setPos(Eigen::Vector3d &position);

  virtual ~Frame();
  void init();
  void destroy();

 private:
  Eigen::Vector3d origin_;
  float arrowBodyLength_;
  float arrowHeadLength_;
  float arrowBodyRadius_;
  float arrowHeadRadius_;

  Eigen::Matrix3d xAxisArrowRotMat_;
  Eigen::Matrix3d yAxisArrowRotMat_;
  Eigen::Matrix3d zAxisArrowRotMat_;

  Obj::Arrow xAxisArrow_;
  Obj::Arrow yAxisArrow_;
  Obj::Arrow zAxisArrow_;
};
} // Obj
} // Graphics
} // RAI

#endif //RAIGRAPHICSOPENGL_FRAME_HPP
