//
// Created by jhwangbo on 17. 5. 3.
//
#include <raiCommon/math/RAI_math.hpp>
#include "raiGraphics/obj/ANYmal.hpp"
#include "raiCommon/TypeDef.hpp"

namespace rai_graphics {
namespace object {

ANYmal::ANYmal():
    base(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_base.dae", 0.001),
    hip_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    thigh_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    shank_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_l.dae", 0.001),
    shank_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_r.dae", 0.001),
    shank_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_l.dae", 0.001),
    shank_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_r.dae", 0.001),
    adapter_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_adapter.dae", 0.001),
    adapter_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_adapter.dae", 0.001),
    adapter_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_adapter.dae", 0.001),
    adapter_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_adapter.dae", 0.001),
    foot_lf(0.035),
    foot_rf(0.035),
    foot_lh(0.035),
    foot_rh(0.035){

  reflectable = true;

  objs.push_back(&base);

  objs.push_back(&hip_lf);
  objs.push_back(&thigh_lf);
  objs.push_back(&shank_lf);

  objs.push_back(&hip_rf);
  objs.push_back(&thigh_rf);
  objs.push_back(&shank_rf);

  objs.push_back(&hip_lh);
  objs.push_back(&thigh_lh);
  objs.push_back(&shank_lh);

  objs.push_back(&hip_rh);
  objs.push_back(&thigh_rh);
  objs.push_back(&shank_rh);

  objs.push_back(&foot_lf);
  objs.push_back(&foot_rf);
  objs.push_back(&foot_lh);
  objs.push_back(&foot_rh);

  objs.push_back(&adapter_lf);
  objs.push_back(&adapter_rf);
  objs.push_back(&adapter_lh);
  objs.push_back(&adapter_rh);

  defaultPose_.resize(21);

  for(auto& pose : defaultPose_)
    pose.setIdentity();

  /// manual adjustment
  /// hip
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[1], M_PI*0.5);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[4], M_PI*0.5);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[4], M_PI);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[7], -M_PI*0.5);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[10], -M_PI*0.5);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[10], M_PI);

  /// thigh
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[5], 0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[5], M_PI);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[5], -0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[11], -0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[11], M_PI);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[11], 0.5*M_PI);

//  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[3], -M_PI*0.09);
//  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[6], -M_PI*0.09);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[9], M_PI*1);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[12], M_PI*1);

  defaultPose_[17](0,3) = 0.1;
  defaultPose_[17](1,3) = -0.02;

  defaultPose_[18](0,3) = 0.1;
  defaultPose_[18](1,3) = 0.02;

  defaultPose_[19](0,3) = -0.1;
  defaultPose_[19](1,3) = -0.02;

  defaultPose_[20](0,3) = -0.1;
  defaultPose_[20](1,3) = 0.02;

}

ANYmal::~ANYmal(){}

void ANYmal::init(){
  for(auto* body: objs)
    body->init();
  for(auto* body: objs)
    shader.push_back(new Shader_basic);
}

void ANYmal::destroy(){
  for(auto* body: objs)
    body->destroy();
  for(auto* sh: shader)
    delete sh;
}

void ANYmal::setPose(std::vector<rai::HomogeneousTransform> &bodyPose) {
  for (int i = 0; i < 17; i++) {
    rai::HomogeneousTransform ht = bodyPose[i] * defaultPose_[i];
    objs[i]->setPose(ht);
  }


  rai::HomogeneousTransform ht;
  ht= bodyPose[3] * defaultPose_[17];
  objs[17]->setPose(ht);

  ht= bodyPose[6] * defaultPose_[18];
  objs[18]->setPose(ht);

  ht= bodyPose[9] * defaultPose_[19];
  objs[19]->setPose(ht);

  ht= bodyPose[12] * defaultPose_[20];
  objs[20]->setPose(ht);
}

} // object
} // rai_graphics

