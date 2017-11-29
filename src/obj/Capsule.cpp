//
// Created by kangd on 29.11.17.
//

#include "raiGraphics/obj/Capsule.hpp"

namespace rai_graphics {
namespace object {

Capsule::Capsule(float r, float l) {
  com = glm::vec3(0, 0, 0);

  int slices = 20;
  int rings = 10;   // hemisphere
  int position = 0;

  float const PHI = 0.5 / (float) (rings - 1);
  float const THETA = 1. / (float) (slices - 1);
  int theta, phi;

  // bottom
  for (phi = 0; phi < rings; phi++) {
    for (theta = 0; theta < rings * 2; theta++) {
      float const x = cos(2 * M_PI * theta * THETA) * sin(M_PI * phi * PHI);
      float const y = sin(2 * M_PI * theta * THETA) * sin(M_PI * phi * PHI);
      float const z = sin(-M_PI_2 + M_PI * phi * PHI);

      positions.push_back(glm::vec3(x * r, y * r, z * r - 0.5 * l));
      normals.push_back(glm::vec3(x * r, y * r, z * r));
    }
  }

  for (phi = 0; phi < rings - 1; phi++) {
    for (theta = 0; theta < rings * 2 - 1; theta++) {
      indices.push_back(phi * rings * 2 + theta);
      indices.push_back((phi + 1) * rings * 2 + (theta + 1));
      indices.push_back(phi * rings * 2 + (theta + 1));
      indices.push_back(phi * rings * 2 + theta);
      indices.push_back((phi + 1) * rings * 2 + theta);
      indices.push_back((phi + 1) * rings * 2 + (theta + 1));
    }
  }

  // side
  position += rings * rings * 2;
  for (int i = 0; i < slices; i++) {
    float theta = 2 * M_PI / slices * i;
    float y = r * std::cos(theta);
    float x = -r * std::sin(theta);
    positions.push_back(glm::vec3(x, y, -0.5 * l));
    normals.push_back(glm::vec3(x, y, 0));
    positions.push_back(glm::vec3(x, y, 0.5 * l));
    normals.push_back(glm::vec3(x, y, 0));
  }

  for (int i = 0; i < slices; i++) {
    indices.push_back(position + 2 * i);
    indices.push_back(position + (2 * (i + 1))%(2*slices));
    indices.push_back(position + 2 * i + 1);

    indices.push_back(position + 2 * i + 1);
    indices.push_back(position + (2 * (i + 1))%(2*slices));
    indices.push_back(position + (2 * (i + 1) + 1)%(2*slices));
  }

  // top
  for (phi = 0; phi < rings; phi++) {
    for (theta = 0; theta < rings * 2; theta++) {
      float const x = cos(2 * M_PI * theta * THETA) * sin(M_PI * phi * PHI);
      float const y = sin(2 * M_PI * theta * THETA) * sin(M_PI * phi * PHI);
      float const z = sin(M_PI_2 - M_PI * phi * PHI);

      positions.push_back(glm::vec3(x * r, y * r, z * r + 0.5 * l));
      normals.push_back(glm::vec3(x * r, y * r, z * r));
    }
  }

  position += slices * 2;
  for (phi = 0; phi < rings - 1; phi++) {
    for (theta = 0; theta < rings * 2 - 1; theta++) {
      indices.push_back(position + phi * rings * 2 + theta);
      indices.push_back(position + (phi + 1) * rings * 2 + (theta + 1));
      indices.push_back(position + phi * rings * 2 + (theta + 1));
      indices.push_back(position + phi * rings * 2 + theta);
      indices.push_back(position + (phi + 1) * rings * 2 + theta);
      indices.push_back(position + (phi + 1) * rings * 2 + (theta + 1));
    }
  }
}

} // object
} // rai_graphics

