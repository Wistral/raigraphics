//
// Created by jhwangbo on 30/01/18.
//

#ifndef RAIGRAPHICSOPENGL_LINES_HPP
#define RAIGRAPHICSOPENGL_LINES_HPP

#include <vector>
namespace rai_graphics {
namespace object {

class Lines {

 public:
  void setColor(float r, float g, float b);
  void draw();
  void clear();
  void addSegment(float x1, float y1, float z1, float x2, float y2, float z2);
  void setLineWidth(float width);

  std::vector<float> color;
  std::vector<float> lines;
  float lineWidth;
};

}
}

#endif //RAIGRAPHICSOPENGL_LINES_HPP
