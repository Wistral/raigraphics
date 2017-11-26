#ifndef SHADER_CHECKERBOARD_INCLUDED_H
#define SHADER_CHECKERBOARD_INCLUDED_H

#include <raiGraphics/obj/CheckerBoard.hpp>
#include "shader.hpp"
#include "glog/logging.h"

namespace rai_graphics {

class Shader_checkerboard : public Shader {
 public:
  Shader_checkerboard();
  ~Shader_checkerboard();

  virtual std::string shaderFileName();
  void Bind();
  void UnBind();
  void Update(Camera *camera,  Light *light, object::CheckerBoard* obj, bool isreflection);
  void Update(Camera *camera,  Light *light, object::SingleBodyObject* obj, bool isreflection){
    LOG(FATAL)<<"model specific shader"<<std::endl;
  }

 protected:
 private:
  static const unsigned int NUM_SHADERS = 2;
  static const unsigned int NUM_UNIFORMS = 4;

  GLuint m_shaders[NUM_SHADERS];
  GLuint m_uniforms[NUM_UNIFORMS];
};

} // rai_graphics

#endif
