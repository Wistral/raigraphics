//
// Created by jhwangbo on 02.12.16.
//

#ifndef RAI_CHECKERBOARD_HALF_HPP
#define RAI_CHECKERBOARD_HALF_HPP

#include "SingleBodyObject.hpp"
#include <vector>
#include "math.h"
#include "rai/common/TypeDef.hpp"

namespace rai_graphics {
namespace object {

class CheckerBoard_half : public SingleBodyObject {

 public:

  CheckerBoard_half(float gridSize, float width, float length, std::vector<float> color1);

};

} // object
} // rai_graphics

#endif //RAI_CHECKERBOARD_HPP
