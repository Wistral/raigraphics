//
// Created by kangd on 16.10.17.
//

#ifndef RAIGRAPHICSOPENGL_RECTANGLE_HPP
#define RAIGRAPHICSOPENGL_RECTANGLE_HPP

#include <SDL2/SDL_ttf.h>
#include "SingleBodyObject.hpp"

namespace rai_graphics {
namespace object {

class Rectangle: public SingleBodyObject {
 public:
  Rectangle(float windowWidth, float windowHeight);
  ~Rectangle();

  void setTranslation(float xpos, float ypos);

  void setSize(float xsize, float ysize);

  void writeText(TTF_Font *font, std::string txt);

  void bindTexture();

  float sizeX_ = 0.5, sizeY_ = 0.5;
  float posX_ = 0, posY_ = 0;
  float windowWidth_, windowHeight_;

 private:
  GLuint tex_;
  bool isTextured = false;
  SDL_Surface* surf;
};

} // object
} // rai_graphics



#endif //RAIGRAPHICSOPENGL_BOX_HPP
