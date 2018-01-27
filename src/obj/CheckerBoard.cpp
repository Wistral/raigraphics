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


void CheckerBoard::drawGridLines() {
  glLineWidth(gridLineWidth);
  // Call only once for all remaining points
  glBegin(GL_LINES);
  // All lines lie in the xy plane.
  for(int i=0; i<width/gridSize_+1; i++) {
    glVertex3f(-width/2.0 + gridSize_*i, -length/2.0, 0.0);    // First endpoint of line
    glVertex3f(-width/2.0 + gridSize_*i, length/2.0, 0.0);    // Second endpoint of line
  }

  // Done drawing points
  for(int i=0; i<length/gridSize_+1; i++) {
    glVertex3f(-width/2.0f, -length/2.0f + gridSize_*i, 0.0);    // First endpoint of line
    glVertex3f(width/2.0f, -length/2.0f + gridSize_*i, 0.0);    // Second endpoint of line
  }
  glEnd();
}

void CheckerBoard::setGridLineWidth(float width) {
  gridLineWidth = width;
}



} // object
} // rai_graphics