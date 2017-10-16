//
// Created by kangd on 16.10.17.
//

#include "raiGraphics/obj/Box.hpp"
rai_graphics::object::Box::Box(float xLength, float yLength, float zLength) {
  // origin is on the center of box

  float xHalfLength = xLength/2;
  float yHalfLength = yLength/2;
  float zHalfLength = zLength/2;

  // up
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,0,1));
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,0,1));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,0,1));
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,0,1));

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  // down
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,0,-1));
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,0,-1));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,0,-1));
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,0,-1));

  indices.push_back(4);
  indices.push_back(5);
  indices.push_back(6);
  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(7);

  // left
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,-1,0));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,-1,0));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,-1,0));
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,-1,0));

  indices.push_back(8);
  indices.push_back(9);
  indices.push_back(10);
  indices.push_back(8);
  indices.push_back(10);
  indices.push_back(11);

  // right
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,1,0));
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(0,1,0));
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,1,0));
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(0,1,0));

  indices.push_back(12);
  indices.push_back(13);
  indices.push_back(14);
  indices.push_back(12);
  indices.push_back(14);
  indices.push_back(15);

  // front
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(1,0,0));
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(1,0,0));
  positions.push_back(glm::vec3(xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(1,0,0));
  positions.push_back(glm::vec3(xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(1,0,0));

  indices.push_back(16);
  indices.push_back(17);
  indices.push_back(18);
  indices.push_back(16);
  indices.push_back(18);
  indices.push_back(19);

  // back
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(-1,0,0));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, zHalfLength));
  normals.push_back(glm::vec3(-1,0,0));
  positions.push_back(glm::vec3(-xHalfLength, -yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(-1,0,0));
  positions.push_back(glm::vec3(-xHalfLength, yHalfLength, -zHalfLength));
  normals.push_back(glm::vec3(-1,0,0));

  indices.push_back(20);
  indices.push_back(21);
  indices.push_back(22);
  indices.push_back(20);
  indices.push_back(22);
  indices.push_back(23);

}
