//
// Created by jhwangbo on 01.05.17.
//

#include <raiGraphics/imp/shader_flat.h>
#include "raiGraphics/obj/CheckerBoard.hpp"

namespace rai_graphics {
namespace object {

CheckerBoard::CheckerBoard(int gridSize, float width, float length, std::vector<float> color1, std::vector<float> color2):
board1(gridSize, width, length, color1), board2(gridSize, width, length, color2){
  objs.push_back(&board1);
  objs.push_back(&board2);
  Eigen::Vector3d pos; pos<<gridSize, 0,0;
  board2.setPos(pos);
}

CheckerBoard::~CheckerBoard(){
}

void CheckerBoard::init(){
  for(auto* ob: objs)
    ob->init();

  for(auto* ob: objs)
    shader.push_back(new Shader_basic);
}

void CheckerBoard::destroy(){
  for(auto* ob: objs)
    ob->destroy();
  for(auto* sh: shader)
    delete sh;
}

} // object
} // rai_graphics