//
// Created by joonho on 03.06.17.
//


#include <rai/common/TypeDef.hpp>
#include "SuperObject.hpp"
#include "Mesh.hpp"
#include "Sphere.hpp"

namespace rai_graphics {
namespace objects {

class Biped_ghost : public SuperObject {

 public:

  Biped_ghost();
  ~Biped_ghost();
  void init();
  void destroy();
  void setPose(std::vector<RAI::HomogeneousTransform> &bodyPose);
  Object* basePtr(){ return &base; }
  Object* footlPtr(){ return &foot_l; }
  Object* footrPtr(){ return &foot_r; }


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

} // objects
} // rai_graphics