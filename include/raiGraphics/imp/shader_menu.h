#ifndef SHADER_MENU_INCLUDED_H
#define SHADER_MENU_INCLUDED_H

#include "shader.hpp"
#include "raiGraphics/imp/shader_menu.h"
#include "raiGraphics/obj/Rectangle.hpp"

namespace rai_graphics {

class Shader_menu : public Shader {
 public:
  Shader_menu();
  ~Shader_menu();

  virtual std::string shaderFileName();
  void Bind();
  void UnBind();
  void Update(object::Rectangle* obj);

  void Update(Camera *camera, Light *light, object::SingleBodyObject* obj, bool isreflection);

 protected:
 private:
  static const unsigned int NUM_SHADERS = 2;
  static const unsigned int NUM_UNIFORMS = 4;

  GLuint m_shaders[NUM_SHADERS];
  GLuint m_uniforms[NUM_UNIFORMS];
};

} // rai_graphics

#endif
