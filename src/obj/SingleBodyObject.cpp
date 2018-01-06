//
// Created by jhwangbo on 17. 4. 29.
//
#include "raiGraphics/obj/SingleBodyObject.hpp"

namespace rai_graphics {
namespace object {

void SingleBodyObject::draw() {
  glBindVertexArray(m_vertexArrayObject);
  glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
}

void SingleBodyObject::init() {
  registerToGPU();
}

void SingleBodyObject::destroy() {
  glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
  glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void SingleBodyObject::setPose(const Eigen::Vector3d &position, const Eigen::Matrix3d &rotationMat) {
  Eigen::Quaternion<double> quat(rotationMat);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPose(const Eigen::Vector3d &position, const Eigen::Vector4d &quaternionAsVector) {
  glm::quat quatglm = glm::quat(quaternionAsVector(0),
                                quaternionAsVector(1),
                                quaternionAsVector(2),
                                quaternionAsVector(3));
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPose(const Eigen::Matrix4d &ht) {
  Eigen::Vector3d pos = ht.topRightCorner(3, 1);
  Eigen::Matrix3d rot = ht.topLeftCorner(3, 3);
  setPose(pos, rot);
}

void SingleBodyObject::setPose(const Eigen::Vector3d &position, const Eigen::Quaterniond &quat) {
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  transform.SetRot(quatglm);
  transform.SetPos(pos);
}

void SingleBodyObject::setPos(const Eigen::Vector3d &position) {
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


void SingleBodyObject::setOri(const Eigen::Vector4d &quaternionAsVector) {
  glm::quat quatglm = glm::quat(quaternionAsVector(0),
                                quaternionAsVector(1),
                                quaternionAsVector(2),
                                quaternionAsVector(3));
  transform.SetRot(quatglm);
}

void SingleBodyObject::setOri(const Eigen::Quaterniond &quat) {
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  transform.SetRot(quatglm);
}

void SingleBodyObject::setOri(const Eigen::Matrix3d &rotationMat) {
  Eigen::Quaternion<double> quat(rotationMat);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  transform.SetRot(quatglm);
}

void SingleBodyObject::setTransform(Transform& trans) {
  transform = trans;
}

void SingleBodyObject::setLightProp(std::vector<float> &amb, std::vector<float> &diff, std::vector<float> &spec, float shine) {
  amb_m = amb;
  diff_m = diff;
  spec_m = spec;
  shininess = shine;
}

void SingleBodyObject::setColor(std::vector<float> colorL) {
  color_ = colorL;
}

void SingleBodyObject::setTransparency(float transparency) {
  transparency_ = transparency;
}

float SingleBodyObject::getTransparency() {
  return transparency_;
}

void SingleBodyObject::setScale(double scale) {
  std::lock_guard<std::mutex> guard(mtx);
  scaleMat_ = glm::scale(glm::vec3(scale, scale, scale));
}

void SingleBodyObject::setScale(double scale1, double scale2, double scale3) {
  scaleMat_ = glm::scale(glm::vec3(scale1, scale2, scale3));
}

glm::mat4 SingleBodyObject::getScale() {
  if(tempTransformOn)
    return scaleMatGhost_;
  else
    return scaleMat_;
}

void SingleBodyObject::getTransform(Transform& trans) {
  if(tempTransformOn)
    trans = transformGhost;
  else
    trans = transform;
}

void SingleBodyObject::setTempTransform(int i) {
  transformGhost = ghosts[i];
  colorGhost_ = {ghostColor[i][0], ghostColor[i][1], ghostColor[i][2]};
  scaleMatGhost_ = glm::scale(glm::vec3(ghostScale[i][0], ghostScale[i][1], ghostScale[i][2]));
}

void SingleBodyObject::getColor(std::vector<float> &clr) {
  if(tempTransformOn)
    clr = colorGhost_;
  else
    clr = color_;
}

void SingleBodyObject::getLightPropAmb(std::vector<float> &amb) {
  amb = amb_m;
}

void SingleBodyObject::getLightPropDiff(std::vector<float> &diff) {
  diff = diff_m;
}

void SingleBodyObject::getLightPropSpec(std::vector<float> &spec) {
  spec = spec_m;
}

void SingleBodyObject::getShiness(float& shine) {
  shine = shininess;
}

const std::vector<glm::vec3>& SingleBodyObject::getVertexPositions() const {
  return positions;
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
  ghostColor.emplace_back(color_[0], color_[1], color_[2]);
  ghostScale.emplace_back(scaleMat_[0][0], scaleMat_[1][1], scaleMat_[2][2]);
}

void SingleBodyObject::addGhost(Eigen::Vector3d &position, Eigen::Quaterniond &quat) {
  Transform ghostTransform;
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  ghostTransform.SetRot(quatglm);
  ghostTransform.SetPos(pos);
  ghosts.push_back(ghostTransform);
  ghostColor.emplace_back(color_[0], color_[1], color_[2]);
  ghostScale.emplace_back(scaleMat_[0][0], scaleMat_[1][1], scaleMat_[2][2]);
}

void SingleBodyObject::addGhost(Eigen::Vector3d &position, Eigen::Quaterniond &quat, std::vector<float> color, std::vector<float> scale) {
  Transform ghostTransform;
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));
  ghostTransform.SetRot(quatglm);
  ghostTransform.SetPos(pos);
  ghosts.push_back(ghostTransform);
  ghostColor.emplace_back(color[0], color[1], color[2]);
  ghostScale.emplace_back(scale[0], scale[1], scale[2]);
}

std::vector<Transform> & SingleBodyObject::getGhosts() {
  return ghosts;
}

void SingleBodyObject::clearGhost() {
  ghosts.clear();
  ghostColor.clear();
  ghostScale.clear();
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
bool SingleBodyObject::isSelectable() const {
  return selectable_;
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