//
// Created by jhwangbo on 17. 4. 28.
//

#ifndef PROJECT_OBJECT_HPP
#define PROJECT_OBJECT_HPP
#include <mutex>

#include "Eigen/Geometry"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "raiGraphics/imp/transform.h"
#include "raiGraphics/imp/Light.hpp"
#include "raiGraphics/obj/Object.hpp"

namespace rai_graphics{
namespace object {

enum MeshBufferPositions {
  POSITION_VB,
  TEXCOORD_VB,
  NORMAL_VB,
  COLOR_VB,
  INDEX_VB
};

class SingleBodyObject: public Object {
 public:

  virtual void draw();

  virtual void init();

  virtual void destroy();

  void setPose(Eigen::Vector3d &position, Eigen::Matrix3d &quaternionAsVector);
  void setPose(Eigen::Vector3d &position, Eigen::Vector4d &quaternionAsVector);
  void setPose(Eigen::Vector3d &position, Eigen::Quaterniond &quat);
  void setPose(Eigen::Matrix4d &ht);

  void setOri(Eigen::Vector4d &quaternionAsVector);
  void setOri(Eigen::Matrix3d &quaternionAsVector);
  void setOri(Eigen::Quaterniond &quat);

  void setPos(Eigen::Vector3d &position);

  void setTransform(Transform& trans);
  void setLightProp(std::vector<float>& amb, std::vector<float>& diff, std::vector<float>& spec, float shine);
  void setColor(std::vector<float> colorL);
  void setTransparency(float transparency);

  float getTransparency();

  glm::mat4 getScale();
  void getTransform(Transform& trans);

  void getColor(std::vector<float>& clr);
  void getLightPropAmb(std::vector<float>& amb);
  void getLightPropDiff(std::vector<float>& diff);
  void getLightPropSpec(std::vector<float>& spec);
  void getShiness(float& shine);
  const float *getVertexPositions() const;
  unsigned long getVertexNumber();
  bool isVisible() {return visible;}

  unsigned int getM_numIndices() const;
  void setVisibility(bool visibility) {visible = visibility;}
  void setScale(double scale);
  void setScale(double scale1,double scale2,double scale3);

  void setTempTransform(Transform& trans);

  void usingTempTransform(bool utt) {tempTransformOn = utt;};

  ShaderType defaultShader = object::RAI_SHADER_BASIC;

  bool reflectable = true;

 protected:
  void registerToGPU();
  Transform transform;
  Transform tempTransform;
  bool tempTransformOn = false;
  glm::mat4 scaleMat_;
  std::vector<float> color_ = {0.7, 0.7, 0.7};
  std::vector<float> amb_m = {0.6, 0.6, 0.6};
  std::vector<float> diff_m = {1.0,1.0,1.0};
  std::vector<float> spec_m = {0.6,0.6,0.6};
  float transparency_ = 1.0;
  float shininess = 50;
  std::vector<glm::vec3> colorsCoords;
  bool visible = true;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> texCoords;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
  static const unsigned int NUM_BUFFERS = 5;
  GLuint m_vertexArrayObject;
  GLuint m_vertexArrayBuffers[NUM_BUFFERS];
  unsigned int m_numIndices;
  std::mutex mtx;


  // TODO code refine(inheritance)
 public:
  virtual void addGhost(Eigen::Vector3d &position);
  virtual void addGhost(Eigen::Vector3d &position, Eigen::Quaterniond &quat);
  void clearGhost();

  std::vector<Transform> & getGhosts();
 protected:
//  void drawSnapshot(Camera *camera,  Light *light, float transparency);
  std::vector<Transform> ghosts;
//  Shader* shader = nullptr;

};

} // object
} // rai_graphics

#endif //PROJECT_OBJECT_HPP
