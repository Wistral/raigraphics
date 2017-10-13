//
// Created by jhwangbo on 02.12.16.
//

#ifndef RAI_CHECKERBOARD_HALF_HPP
#define RAI_CHECKERBOARD_HALF_HPP

#include "Object.hpp"
#include <vector>
#include "math.h"
#include "rai/common/TypeDef.hpp"

namespace rai_graphics {
namespace objects {

class CheckerBoard_half : public Object {

 public:

  CheckerBoard_half(int gridSize, float width, float length, std::vector<float> color1);

};

} // objects
} // rai_graphics

#endif //RAI_CHECKERBOARD_HPP
