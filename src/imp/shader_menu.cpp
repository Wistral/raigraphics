#include "raiGraphics/config.hpp"
#include "raiGraphics/imp/shader_menu.h"
#include <iostream>
#include <fstream>
#include <raiGraphics/obj/Rectangle.hpp>
#include "raiCommon/rai_utils.hpp"

namespace rai_graphics {

Shader_menu::Shader_menu() {
  m_program = glCreateProgram();
  m_shaders[0] = CreateShader(LoadShader(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/" + shaderFileName() + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(std::string(RAI_GRAPHICS_ROOT_PATH) + "/res/" + shaderFileName() + ".fs"), GL_FRAGMENT_SHADER);

  for (unsigned int i = 0; i < NUM_SHADERS; i++)
    glAttachShader(m_program, m_shaders[i]);

  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
}

std::string Shader_menu::shaderFileName(){
  return "menuShader";
}

Shader_menu::~Shader_menu() {
  for (unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader_menu::Bind() {
  glUseProgram(m_program);
}

void Shader_menu::UnBind() {
  glUseProgramObjectARB(0);
}


void Shader_menu::Update(object::Rectangle* obj) {
  glDisable(GL_CLIP_DISTANCE0);
  glDisable(GL_CLIP_DISTANCE1);
  glDisable(GL_CLIP_DISTANCE2);
  glDisable(GL_CLIP_DISTANCE3);
  glDisable(GL_CLIP_DISTANCE4);
  glUniform1f(glGetUniformLocation(m_program,"xscale"),obj->sizeX_);
  glUniform1f(glGetUniformLocation(m_program,"yscale"),obj->sizeY_);
  glUniform1f(glGetUniformLocation(m_program,"xpos"),obj->posX_);
  glUniform1f(glGetUniformLocation(m_program,"ypos"),obj->posY_);
  glUniform1f(glGetUniformLocation(m_program,"tex"),0);
  glUniform1f(glGetUniformLocation(m_program,"transparency"),obj->getTransparency());
}

void Shader_menu::Update(Camera *camera, Light *light, object::SingleBodyObject* obj){
  RAIFATAL("unsupported update method");
}

void Shader_menu::UpdateForReflection(Camera *camera, Light *light, object::SingleBodyObject* obj, object::CheckerBoard* chk){
  RAIFATAL("cannot draw reflection of text");
}


} // rai_graphics