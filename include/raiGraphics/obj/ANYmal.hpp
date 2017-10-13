//
// Created by jhwangbo on 17. 5. 3.
//

#ifndef RAI_ANYMAL_HPP
#define RAI_ANYMAL_HPP

#include <rai/common/TypeDef.hpp>
#include "SuperObject.hpp"
#include "Mesh.hpp"

namespace RAI {
namespace rai_graphics {
namespace objects {

class ANYmal : public SuperObject {

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  ANYmal();
  ~ANYmal();
  void init();
  void destroy();
  void setPose(std::vector<HomogeneousTransform> &bodyPose);
  RAI::rai_graphics::objects::Object* basePtr(){ return &base; }

 private:

  RAI::rai_graphics::objects::Mesh base;
  RAI::rai_graphics::objects::Mesh hip_lf;
  RAI::rai_graphics::objects::Mesh hip_rf;
  RAI::rai_graphics::objects::Mesh hip_lh;
  RAI::rai_graphics::objects::Mesh hip_rh;

  RAI::rai_graphics::objects::Mesh thigh_lf;
  RAI::rai_graphics::objects::Mesh thigh_rf;
  RAI::rai_graphics::objects::Mesh thigh_lh;
  RAI::rai_graphics::objects::Mesh thigh_rh;

  RAI::rai_graphics::objects::Mesh shank_lf;
  RAI::rai_graphics::objects::Mesh shank_rf;
  RAI::rai_graphics::objects::Mesh shank_lh;
  RAI::rai_graphics::objects::Mesh shank_rh;

  RAI::rai_graphics::objects::Mesh foot_lf;
  RAI::rai_graphics::objects::Mesh foot_rf;
  RAI::rai_graphics::objects::Mesh foot_lh;
  RAI::rai_graphics::objects::Mesh foot_rh;
  std::vector<HomogeneousTransform> defaultPose_;
};

}
}
}


#endif //RAI_ANYMAL_HPP
