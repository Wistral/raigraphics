//
// Created by jhwangbo on 30/01/18.
//

#include <raiCommon/utils/rai_message_logger/rai_message.hpp>
#include <GL/gl.h>
#include "raiGraphics/obj/Lines.hpp"

namespace rai_graphics {
namespace object {

  Lines::Lines() {
    color = {1,1,1};
    lineWidth=2.0f;
  }

  void Lines::setColor(float r, float g, float b) {
    color = {r,g,b};
  }

  void Lines::draw() {
    RAIFATAL_IF(lines.size()%6 != 0, "the number of line points is not even");

    glLineWidth(lineWidth);
    glBegin(GL_LINES);

    for(int i=0; i<lines.size(); i+=6) {
      glVertex3f(lines[i], lines[i+1], lines[i+2]);    // First endpoint of line
      glVertex3f(lines[i+3], lines[i+4], lines[i+5]);    // Second endpoint of line
    }
    glEnd();
  }

  void Lines::clear() {
    lines.clear();
  }

  void Lines::setLineWidth(float width) {
    lineWidth = width;
  }

  void Lines::addSegment(float x1, float y1, float z1, float x2, float y2, float z2) {
    lines.push_back(x1);
    lines.push_back(y1);
    lines.push_back(z1);
    lines.push_back(x2);
    lines.push_back(y2);
    lines.push_back(z2);
  }


}
}