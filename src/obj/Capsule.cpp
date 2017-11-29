//
// Created by kangd on 29.11.17.
//

#include "raiGraphics/obj/Capsule.hpp"

namespace rai_graphics {
namespace object {

Capsule::Capsule(float r, float l) {
  com = glm::vec3(0, 0, 0);

  int slices = 20;
  int rings = slices/2;   // hemisphere
  int position = 0;
  int position2 = 0;

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
      indices.push_back((phi + 1) * rings * 2 + (theta + 1));
      indices.push_back((phi + 1) * rings * 2 + theta);
      indices.push_back(phi * rings * 2 + (theta + 1));
      indices.push_back((phi + 1) * rings * 2 + theta);
      indices.push_back(phi * rings * 2 + theta);
      indices.push_back(phi * rings * 2 + (theta + 1));
    }
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

  position += rings * rings * 2;
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

  // side
  position = (rings * 2) * (rings - 1);
  position2 = rings * rings * 2 + (rings * 2) * (rings - 1);
  for (int i = 0; i < slices-1; i++) {
    indices.push_back(position + i);
    indices.push_back(position + i + 1);
    indices.push_back(position2 + i + 1);

    indices.push_back(position + i);
    indices.push_back(position2 + i + 1);
    indices.push_back(position2 + i);
  }


}

} // object
} // rai_graphics

