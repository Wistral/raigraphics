//
// Created by jhwangbo on 01.05.17.
//

#include <raiGraphics/imp/shader_flat.h>
#include "raiGraphics/obj/CheckerBoard.hpp"

namespace rai_graphics {
namespace object {

CheckerBoard::CheckerBoard(float gridSize,
                           float wid,
                           float leng,
                           float reflectanceI,
                           std::vector<float> color1,
                           std::vector<float> color2) {
  width = wid;
  length = leng;

  com = glm::vec3(0,0,0);
  col1 = {color1[0], color1[1], color1[2]};
  col2 = {color2[0], color2[1], color2[2]};
  gridSize_ = gridSize;
  transparency_ = 1 - reflectanceI;

  positions.emplace_back(-wid/2.0, -leng/2.0, 0);
  positions.emplace_back(wid/2.0, -leng/2.0, 0);
  positions.emplace_back(wid/2.0, leng/2.0, 0);
  positions.emplace_back(-wid/2.0, leng/2.0, 0);

  normals.emplace_back(0,0,1);
  normals.emplace_back(0,0,1);
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(3);
  indices.push_back(0);
}



} // object
} // rai_graphics