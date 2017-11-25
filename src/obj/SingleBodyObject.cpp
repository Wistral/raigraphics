//
// Created by jhwangbo on 17. 4. 29.
//
#include "raiGraphics/obj/SingleBodyObject.hpp"

namespace rai_graphics {
namespace object {

void SingleBodyObject::draw() {
  std::lock_guard<std::mutex> guard(mtx);
  glBindVertexArray(m_vertexArrayObject);
  glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
}

void SingleBodyObject::init() {
  std::lock_guard<std::mutex> guard(mtx);
  registerToGPU();
}

void SingleBodyObject::destroy() {
  glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
  glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void SingleBodyObject::setPose(Eigen::Vector3d &position, Eigen::Matrix3d &rotationMat) {
  std::lock_guard<std::mutex> guard(mtx);
  Eigen::Quaternion<double> quat(rotationMat);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPose(Eigen::Vector3d &position, Eigen::Vector4d &quaternionAsVector) {
  std::lock_guard<std::mutex> guard(mtx);
  glm::quat quatglm = glm::quat(quaternionAsVector(0),
                                quaternionAsVector(1),
                                quaternionAsVector(2),
                                quaternionAsVector(3));
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPose(Eigen::Matrix4d &ht) {
  Eigen::Vector3d pos = ht.topRightCorner(3, 1);
  Eigen::Matrix3d rot = ht.topLeftCorner(3, 3);
  setPose(pos, rot);
}

void SingleBodyObject::setPose(Eigen::Vector3d &position, Eigen::Quaterniond &quat) {
  std::lock_guard<std::mutex> guard(mtx);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPos(Eigen::Vector3d &position) {
  std::lock_guard<std::mutex> guard(mtx);
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetPos(pos);
}

void SingleBodyObject::setPos(double x, double y, double z) {
  (*transform.GetPos()).x = float(x);
  (*transform.GetPos()).y = float(y);
  (*transform.GetPos()).z = float(z);
}

void SingleBodyObject::setOri(double w, double x, double y, double z){
  (*transform.GetRot()).w = float(w);
  (*transform.GetRot()).x = float(x);
  (*transform.GetRot()).y = float(y);
  (*transform.GetRot()).z = float(z);
}


void SingleBodyObject::setOri(Eigen::Vector4d &quaternionAsVector) {
  std::lock_guard<std::mutex> guard(mtx);
  glm::quat quatglm = glm::quat(quaternionAsVector(0),
                                quaternionAsVector(1),
                                quaternionAsVector(2),
                                quaternionAsVector(3));
  transform.SetRot(quatglm);
}

void SingleBodyObject::setOri(Eigen::Quaterniond &quat) {
  std::lock_guard<std::mutex> guard(mtx);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  transform.SetRot(quatglm);
}

void SingleBodyObject::setOri(Eigen::Matrix3d &rotationMat) {
  std::lock_guard<std::mutex> guard(mtx);
  Eigen::Quaternion<double> quat(rotationMat);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  transform.SetRot(quatglm);
}

void SingleBodyObject::setTransform(Transform& trans) {
  transform = trans;
}

void SingleBodyObject::setLightProp(std::vector<float> &amb, std::vector<float> &diff, std::vector<float> &spec, float shine) {
  std::lock_guard<std::mutex> guard(mtx);
  amb_m = amb;
  diff_m = diff;
  spec_m = spec;
  shininess = shine;
}

void SingleBodyObject::setColor(std::vector<float> colorL) {
  std::lock_guard<std::mutex> guard(mtx);
  color_ = colorL;
}

void SingleBodyObject::setTransparency(float transparency) {
  std::lock_guard<std::mutex> guard(mtx);
  transparency_ = transparency;
}

float SingleBodyObject::getTransparency() {
  std::lock_guard<std::mutex> guard(mtx);
  return transparency_;
}

void SingleBodyObject::setScale(double scale) {
  std::lock_guard<std::mutex> guard(mtx);
  scaleMat_ = glm::scale(glm::vec3(scale, scale, scale));
}

void SingleBodyObject::setScale(double scale1, double scale2, double scale3) {
  std::lock_guard<std::mutex> guard(mtx);
  scaleMat_ = glm::scale(glm::vec3(scale1, scale2, scale3));
}

glm::mat4 SingleBodyObject::getScale() {
  std::lock_guard<std::mutex> guard(mtx);
  return scaleMat_;
}

void SingleBodyObject::getTransform(Transform& trans) {
  std::lock_guard<std::mutex> guard(mtx);
  if(tempTransformOn)
    trans = tempTransform;
  else
    trans = transform;
}

void SingleBodyObject::setTempTransform(Transform& trans) {
  tempTransform = trans;
}

void SingleBodyObject::getColor(std::vector<float> &clr) {
  std::lock_guard<std::mutex> guard(mtx);
  clr = color_;
}

void SingleBodyObject::getLightPropAmb(std::vector<float> &amb) {
  std::lock_guard<std::mutex> guard(mtx);
  amb = amb_m;
}

void SingleBodyObject::getLightPropDiff(std::vector<float> &diff) {
  std::lock_guard<std::mutex> guard(mtx);
  diff = diff_m;
}

void SingleBodyObject::getLightPropSpec(std::vector<float> &spec) {
  std::lock_guard<std::mutex> guard(mtx);
  spec = spec_m;
}

void SingleBodyObject::getShiness(float& shine) {
  std::lock_guard<std::mutex> guard(mtx);
  shine = shininess;
}

const float *SingleBodyObject::getVertexPositions() const {
  return &positions[0].x;
}

unsigned long SingleBodyObject::getVertexNumber() {
  return positions.size();
}
void SingleBodyObject::registerToGPU() {
  m_numIndices = indices.size();

  glGenVertexArrays(1, &m_vertexArrayObject);
  glBindVertexArray(m_vertexArrayObject);

  glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(positions[0]) * positions.size(),
               &positions[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(texCoords[0]) * texCoords.size(),
               &texCoords[0],
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorsCoords[0]) * colorsCoords.size(), &colorsCoords[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(indices[0]) * indices.size(),
               &indices[0],
               GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void SingleBodyObject::addGhost(Eigen::Vector3d &position) {
  Transform ghostTransform;
  glm::vec3 pos(position(0), position(1), position(2));
  ghostTransform.SetPos(pos);

  ghosts.push_back(ghostTransform);
}
void SingleBodyObject::addGhost(Eigen::Vector3d &position, Eigen::Quaterniond &quat) {
  Transform ghostTransform;
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  ghostTransform.SetRot(quatglm);
  ghostTransform.SetPos(pos);

  ghosts.push_back(ghostTransform);
}

std::vector<Transform> & SingleBodyObject::getGhosts() {
  return ghosts;
}

void SingleBodyObject::clearGhost() {
  ghosts.clear();
}

void SingleBodyObject::highlight() {
  amb_m_orig[0] = amb_m[0];
  amb_m_orig[1] = amb_m[1];
  amb_m_orig[2] = amb_m[2];

  amb_m[0] = 3.0f;
  amb_m[1] = 3.0f;
  amb_m[2] = 3.0f;
  transparencyOrig_ = transparency_;
  transparency_ = 0.9;
}

void SingleBodyObject::deHighlight() {
  amb_m[0] = amb_m_orig[0];
  amb_m[1] = amb_m_orig[1];
  amb_m[2] = amb_m_orig[2];
  transparency_ = transparencyOrig_;
}

//void SingleBodyObject::drawSnapshot(Camera *camera, Light *light, float transparency) {
//  setTransparency(transparency);
//  shader->Bind();
//  shader->Update(camera, light, this);
//  draw();
//  shader->UnBind();
//}

} // object
} // rai_graphics