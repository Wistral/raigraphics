//
// Created by joonho on 19.05.17.
//

#include "raiGraphics/obj/Cone.hpp"

namespace rai_graphics {
namespace object {

Cone::Cone(float r, float l) {
  com = glm::vec3(0,0,l*.25f);

  int slices = 20;
//  bottom
  positions.emplace_back(glm::vec3(0, 0, 0));
  normals.emplace_back(glm::vec3(0, 0, -1));

  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r * std::cos(theta);
    float x = -r * std::sin(theta);
    positions.emplace_back(glm::vec3(x, y, 0));
    normals.emplace_back(glm::vec3(0,0,-1));
  }
  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(0);
    indices.push_back(i % slices + 1);
    indices.push_back(i);
  }

  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r * std::cos(theta);
    float x = -r * std::sin(theta);
    positions.emplace_back(glm::vec3(x, y, 0));
    normals.emplace_back(glm::vec3(x, y, -l/(sqrt(r*r + l*l)+r)));
  }
//Top
  positions.emplace_back(glm::vec3(0, 0, l));
  normals.emplace_back(glm::vec3(0, 0, 1));

  for (int i = 1; i < slices + 1; i++) {
    indices.push_back(2*slices + 1);
    indices.push_back(slices+i);
    indices.push_back(slices + i % slices + 1);
  }
}

} // object
} // rai_graphics
