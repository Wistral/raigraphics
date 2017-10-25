//
// Created by joonho on 19.05.17.
//
#include "raiGraphics/obj/Arrow.hpp"

namespace rai_graphics {
namespace object {

Arrow::Arrow(float r1, float r2, float l1, float l2) {
  int slices = 20;
  int position = 0;
//  bottom
  positions.push_back(glm::vec3(0, 0, 0));
  normals.push_back(glm::vec3(-1, 0, 0));

  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r1 * std::cos(theta);
    float z = r1 * std::sin(theta);
    positions.push_back(glm::vec3(0, y, z));
    normals.push_back(glm::vec3(-r1, y, z));
  }
  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(0);
    indices.push_back(i % slices + 1);
    indices.push_back(i);
  }
  position = slices;
//side
  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r1 * std::cos(theta);
    float z = r1 * std::sin(theta);
    positions.push_back(glm::vec3(l1, y, z));
    normals.push_back(glm::vec3(0, y, z));
  }
  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(i);
    indices.push_back(i % slices + 1);
    indices.push_back(position+ i);

    indices.push_back(position + i);
    indices.push_back(i % slices + 1);
    indices.push_back(position + i % slices + 1);
  }

//Head bottom
  positions.push_back(glm::vec3(l1, 0, 0));
  normals.push_back(glm::vec3(1 , 0, 0));
  position += slices + 1;

  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r2 * std::cos(theta);
    float z = r2 * std::sin(theta);
    positions.push_back(glm::vec3(l1, y, z));
    normals.push_back(glm::vec3(l2, y, z));
  }
  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(position);
    indices.push_back(i % slices + 1+ position);
    indices.push_back(i + position);
  }

  //Head
  positions.push_back(glm::vec3(l1 + l2, 0, 0));
  normals.push_back(glm::vec3(1 , 0, 0));

  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(i +position);
    indices.push_back(i % slices + 1 +position);
    indices.push_back(position + slices + 1);
  }
}

void Arrow::representVector(Eigen::Vector3d &directionVector) {
  Eigen::Quaterniond rotation = quaternionForDirectionVector(directionVector);
  setOri(rotation);
}
void Arrow::representVector(Eigen::Vector3d &origin, Eigen::Vector3d &directionVector) {
  Eigen::Quaterniond rotation = quaternionForDirectionVector(directionVector);
  setPose(origin, rotation);
}

Eigen::Quaterniond Arrow::quaternionForDirectionVector(const Eigen::Vector3d &directionVector) const {
  double theta = atan2(directionVector(1), directionVector(0));
  double sqrt_x2y2 = sqrt(directionVector(0) * directionVector(0)
                              + directionVector(1) * directionVector(1));
  double phi = atan2(directionVector(2), sqrt_x2y2);

  Eigen::Quaterniond rotationZ(cos(theta/2), 0, 0, sin(theta/2));
  Eigen::Quaterniond rotationY(cos(phi/2), 0, -sin(phi/2), 0);
  Eigen::Quaterniond rotation = rotationZ * rotationY;
  return rotation;
}
void Arrow::addGhostWithVector(Eigen::Vector3d &position, Eigen::Vector3d &directionVector) {
  Eigen::Quaterniond quat = quaternionForDirectionVector(directionVector);
  glm::quat quatglm = glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
  glm::vec3 pos(position(0), position(1), position(2));

  Transform ghostTransform;
  ghostTransform.SetRot(quatglm);
  ghostTransform.SetPos(pos);
  ghosts.push_back(ghostTransform);
}

} // object
}