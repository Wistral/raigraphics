//
// Created by jhwangbo on 17. 5. 3.
//
#include <raiCommon/math/RAI_math.hpp>
#include "raiGraphics/obj/ANYmal.hpp"
#include "raiCommon/TypeDef.hpp"

namespace rai_graphics {
namespace object {

ANYmal::ANYmal():
    base(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_base_1_2.dae", 0.001),
    hip_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    hip_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_hip.dae", 0.001),
    thigh_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    thigh_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_thigh.dae", 0.001),
    shank_lf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_2_LF.dae", 0.001),
    shank_rf(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_2_RF.dae", 0.001),
    shank_lh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_2_LH.dae", 0.001),
    shank_rh(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/ANYmal/anymal_shank_2_RH.dae", 0.001),
    foot_lf(0.025, false),
    foot_rf(0.025, false),
    foot_lh(0.025, false),
    foot_rh(0.025, false) {

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

  defaultPose_.resize(17);
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
  for (int i = 0; i < objs.size(); i++) {
    rai::HomogeneousTransform ht = bodyPose[i] * defaultPose_[i];
    objs[i]->setPose(ht);
  }
}

} // object
} // rai_graphics

