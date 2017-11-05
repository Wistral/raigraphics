//
// Created by jhwangbo on 01.05.17.
//

#ifndef RAI_CHECKERBOARD_HPP
#define RAI_CHECKERBOARD_HPP

#include "MultiBodyObject.hpp"
#include "CheckerBoard_half.hpp"

namespace rai_graphics {
namespace object {

class CheckerBoard : public MultiBodyObject {

 public:

  CheckerBoard(float gridSize, float width, float length, float reflectanceI, std::vector<float> color1={0,0,0}, std::vector<float> color2={1,1,1});
  void init();
  void destroy();

  float reflectance;
  CheckerBoard_half board1, board2;
};

} // object
} // rai_graphics


#endif //RAI_CHECKERBOARD_HPP
