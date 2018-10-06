//
// Created by jhwangbo on 17. 5. 3.
//
#include <raiCommon/math/RAI_math.hpp>
#include "raiGraphics/config.hpp"
#include "raiGraphics/obj/Quadrotor.hpp"
#include "raiCommon/TypeDef.hpp"

namespace rai_graphics {
namespace object {

Quadrotor::Quadrotor(float scale):
    base(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/CAD_AscTec_Hummingbird.dae", scale*0.003),
    prop1(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/propeller_ccw.dae", scale*0.3),
    prop2(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/propeller_cw.dae", scale*0.3),
    prop3(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/propeller_ccw.dae", scale*0.3),
    prop4(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/propeller_cw.dae", scale*0.3),
    brain(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/Quadrotor/RLBrain_Assembly.dae", scale*0.003){

  objs.push_back(&base);
  objs.push_back(&prop1);
  objs.push_back(&prop2);
  objs.push_back(&prop3);
  objs.push_back(&prop4);
  objs.push_back(&brain);

  defaultPose_.resize(objs.size());

  for(auto& pose : defaultPose_)
    pose.setIdentity();

  /// manual adjustment
  //Hummingbird frame origin
  defaultPose_[0](0, 3) = 0.11 * scale;
  defaultPose_[0](1, 3) = -0.06 * scale;
  //RLBrain
  defaultPose_[5](0, 3) = -0.13 * scale;
  defaultPose_[5](1, 3) = 0.02 * scale;
  defaultPose_[5](2, 3) = -0.22 * scale;
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[5], M_PI);
  rai::Math::MathFunc::rotateHTabout_y_axis(defaultPose_[5], 0.25*M_PI);

  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[1], 0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[2], 0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[3], 0.5*M_PI);
  rai::Math::MathFunc::rotateHTabout_x_axis(defaultPose_[4], 0.5*M_PI);

  for(int i = 1; i<5; i++){
    defaultPose_[i](0, 3) = 0.5*cos(M_PI/2*(i-1))*scale;
    defaultPose_[i](1, 3) = -0.06*scale;
    defaultPose_[i](2, 3) = 0.5*sin(M_PI/2*(i-1))*scale;
  }


  /// Set Colors
  prop1.setColor({0.0, 0.0, 1.0});
  prop2.setColor({0.0, 0.0, 1.0});
  prop3.setColor({1.0, 0.0, 0.0});
  prop4.setColor({1.0, 0.0, 0.0});
  brain.setColor({1.0, 1.0, 0.0});
}

Quadrotor::~Quadrotor(){}

void Quadrotor::init(){
  for(auto* body: objs)
    body->init();
  for(auto* body: objs)
    shader.push_back(new Shader_basic);
}

void Quadrotor::destroy(){
  for(auto* body: objs)
    body->destroy();
  for(auto* sh: shader)
    delete sh;
}

void Quadrotor::setPose(Eigen::Matrix4d &ht) {

  for (int i = 0; i < objs.size(); i++) {
    rai::HomogeneousTransform objPose = ht * defaultPose_[i];
    objs[i]->setPose(objPose);
  }
}

void Quadrotor::setPose(Eigen::Vector3d &position, Eigen::Vector4d &quat) {

  Eigen::Matrix4d basePose;

  basePose.topLeftCorner(3,3) = rai::Math::MathFunc::quatToRotMat(quat);
  basePose.topRightCorner(3,1) = position;

  for (int i = 0; i < objs.size(); i++) {
    rai::HomogeneousTransform objPose = basePose * defaultPose_[i];
    objs[i]->setPose(objPose);
  }
}

void Quadrotor::spinRotors(){
  float speed[4] = {1, -1, 1, -1};
  Eigen::Matrix4d rotorRotate;

  for(int i = 1; i<5; i++){
    rotorRotate << cos(speed[i-1]),   sin(speed[i-1]), 0, 0,
                  -sin(speed[i-1]),   cos(speed[i-1]), 0, 0,
                               0,               0, 1, 0,
                               0,               0, 0, 1;
    defaultPose_[i] = defaultPose_[i]*rotorRotate;
  }

}
} // object
} // rai_graphics

