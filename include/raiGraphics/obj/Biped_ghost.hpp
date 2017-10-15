//
// Created by joonho on 03.06.17.
//


#include <rai/common/TypeDef.hpp>
#include "MultiBodyObject.hpp"
#include "Mesh.hpp"
#include "Sphere.hpp"

namespace rai_graphics {
namespace object {

class Biped_ghost : public MultiBodyObject {

 public:

  Biped_ghost();
  ~Biped_ghost();
  void init();
  void destroy();
  void setPose(std::vector<RAI::HomogeneousTransform> &bodyPose);
  SingleBodyObject* basePtr(){ return &base; }
  SingleBodyObject* footlPtr(){ return &foot_l; }
  SingleBodyObject* footrPtr(){ return &foot_r; }


 private:

  Mesh base;
  Mesh haa_l;
  Mesh hfe_l;
  Mesh thigh_l;
  Mesh shank_l;
// Mesh afe_l;

  Mesh haa_r;
  Mesh hfe_r;
  Mesh thigh_r;
  Mesh shank_r;
//  Mesh afe_r;

  Sphere foot_l;
  Sphere foot_r;

  std::vector<RAI::HomogeneousTransform> defaultPose_;
};

} // object
} // rai_graphics