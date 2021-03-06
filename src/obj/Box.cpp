//
// Created by kangd on 16.10.17.
//

#include "raiGraphics/obj/Box.hpp"

namespace rai_graphics{
namespace object{

Box::Box(float xLength, float yLength, float zLength, bool isSelectable) {
  // origin is on the center of box
  com = glm::vec3(0,0,0);

  defaultShader = object::RAI_SHADER_FLAT;

  float xHalfLength = xLength/2;
  float yHalfLength = yLength/2;
  float zHalfLength = zLength/2;

  // up
  positions.emplace_back(glm::vec3(xHalfLength, yHalfLength, zHalfLength));
  positions.emplace_back(glm::vec3(-xHalfLength, yHalfLength, zHalfLength));
  positions.emplace_back(glm::vec3(-xHalfLength, -yHalfLength, zHalfLength));
  positions.emplace_back(glm::vec3(xHalfLength, -yHalfLength, zHalfLength));

  // down
  positions.emplace_back(glm::vec3(xHalfLength, yHalfLength, -zHalfLength));
  positions.emplace_back(glm::vec3(-xHalfLength, yHalfLength, -zHalfLength));
  positions.emplace_back(glm::vec3(-xHalfLength, -yHalfLength, -zHalfLength));
  positions.emplace_back(glm::vec3(xHalfLength, -yHalfLength, -zHalfLength));

  // up
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(0);

  // down
  indices.push_back(4);
  indices.push_back(7);
  indices.push_back(6);
  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(5);

  // front
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(4);
  indices.push_back(3);
  indices.push_back(7);
  indices.push_back(4);

  // back
  indices.push_back(2);
  indices.push_back(5);
  indices.push_back(6);
  indices.push_back(1);
  indices.push_back(5);
  indices.push_back(2);

  // right
  indices.push_back(0);
  indices.push_back(5);
  indices.push_back(1);
  indices.push_back(0);
  indices.push_back(4);
  indices.push_back(5);

  // left
  indices.push_back(6);
  indices.push_back(7);
  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(6);
  indices.push_back(3);

  // selectable
  selectable_ = isSelectable;
}

}
}

