//
// Created by joonho on 07.05.17.
//

#ifndef RAI_BIPED_HPP
#define RAI_BIPED_HPP
#include <rai/common/TypeDef.hpp>
#include "SuperObject.hpp"
#include "Mesh.hpp"
#include "Sphere.hpp"

namespace rai_graphics {
namespace objects {

class Biped : public SuperObject {

 public:

  Biped();
  ~Biped();
  void init();
  void destroy();
  void setPose(std::vector<HomogeneousTransform> &bodyPose);
  RAI::rai_graphics::objects::Object* basePtr(){ return &base; }

 private:


  RAI::rai_graphics::objects::Mesh base;
  RAI::rai_graphics::objects::Mesh haa_l;
  RAI::rai_graphics::objects::Mesh hfe_l;
  RAI::rai_graphics::objects::Mesh thigh_l;
  RAI::rai_graphics::objects::Mesh shank_l;
  RAI::rai_graphics::objects::Mesh afe_l;

  RAI::rai_graphics::objects::Mesh haa_r;
  RAI::rai_graphics::objects::Mesh hfe_r;
  RAI::rai_graphics::objects::Mesh thigh_r;
  RAI::rai_graphics::objects::Mesh shank_r;
  RAI::rai_graphics::objects::Mesh afe_r;

  RAI::rai_graphics::objects::Mesh foot_l;
  RAI::rai_graphics::objects::Mesh foot_r;

  std::vector<HomogeneousTransform> defaultPose_;
};

} // objects
} // rai_graphics

#endif //RAI_BIPED_HPP
