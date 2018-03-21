//
// Created by kangd on 16.10.17.
//

#include <SDL2/SDL_system.h>
#include <SDL2/SDL_ttf.h>
#include "raiGraphics/obj/Rectangle.hpp"
#include "raiCommon/rai_utils.hpp"

namespace rai_graphics {
namespace object {

Rectangle::Rectangle(float windowWidth, float windowHeight) {

  windowHeight_ = windowHeight;
  windowWidth_ = windowWidth;
  // origin is on the center of box
  com = glm::vec3(0, 0, 0);
  defaultShader = object::RAI_SHADER_BASIC;

  // up
  positions.emplace_back(glm::vec3(0, 0, 0));
  positions.emplace_back(glm::vec3(1.0f, 0, 0));
  positions.emplace_back(glm::vec3(0, 1.0f, 0));
  positions.emplace_back(glm::vec3(1.0f, 1.0f, 0));

  // up
  texCoords.emplace_back(glm::vec2(0,1));
  texCoords.emplace_back(glm::vec2(1,1));
  texCoords.emplace_back(glm::vec2(0,0));
  texCoords.emplace_back(glm::vec2(1,0));

  // up
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(2);
  indices.push_back(1);
  indices.push_back(3);
}

Rectangle::~Rectangle(){
  if(isTextured) {
    SDL_FreeSurface(surf);
    glDeleteTextures(1, &tex_);
  }
}

void Rectangle::setTranslation(float xpos, float ypos){
  posX_ = xpos/windowWidth_*2.0f; posY_ = ypos/windowHeight_*2.0f;
}

void Rectangle::setSize(float xsize, float ysize){
  sizeX_ = xsize/windowWidth_*2.0f; sizeY_ = ysize/windowHeight_*2.0f;
}

void Rectangle::setTextWrap(int tw){
  wrapLength = tw;
}


void Rectangle::writeText(const std::vector<TTF_Font *>& font, const std::string& txt) {

  if(txt.empty()){
    setSize(0,0);
    return;
  }

  if(isTextured) {
    SDL_FreeSurface(surf);
    glDeleteTextures(1, &tex_);
  }
  SDL_Color color = {255,255,255,255};
  surf = TTF_RenderText_Blended_Wrapped(font[fontSize], txt.c_str(), color, wrapLength);
  glGenTextures(1, &tex_);
  glBindTexture(GL_TEXTURE_2D, tex_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);

  setSize(surf->w, surf->h);
  isTextured = true;
}

void Rectangle::bindTexture() {
  if(!isTextured)
    return;

  glBindTexture(GL_TEXTURE_2D, tex_);
}

void Rectangle::setFontSize(int size) {
  RAIWARN_IF(size<0 || size>5, "font size from 0~5 are supported");
  fontSize = size%6;
}


}
}
