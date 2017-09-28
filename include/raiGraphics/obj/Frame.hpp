//
// Created by kangd on 28.09.17.
//

#ifndef RAIGRAPHICSOPENGL_FRAME_HPP
#define RAIGRAPHICSOPENGL_FRAME_HPP

#include "SuperObject.hpp"
#include "Arrow.hpp"
#include <vector>

namespace RAI {
namespace Graphics {
namespace Obj {

class Frame : public SuperObject {

 public:
  Frame(bool colorYn);
  virtual ~Frame();
  void init();
  void destroy();

  Obj::Arrow xAxisArrow;
  Obj::Arrow yAxisArrow;
  Obj::Arrow zAxisArrow;
};
} // Obj
} // Graphics
} // RAI

#endif //RAIGRAPHICSOPENGL_FRAME_HPP
