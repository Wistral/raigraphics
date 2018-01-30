#include "raiGraphics/imp/shader_line.h"
#include <iostream>
#include <fstream>

namespace rai_graphics {

Shader_line::Shader_line() {
  m_program = glCreateProgram();
  m_shaders[0] = CreateShader(LoadShader(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/" + shaderFileName() + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/" + shaderFileName() + ".fs"), GL_FRAGMENT_SHADER);

  for (unsigned int i = 0; i < NUM_SHADERS; i++)
    glAttachShader(m_program, m_shaders[i]);

  glBindAttribLocation(m_program, 0, "position");
  glBindAttribLocation(m_program, 1, "texCoord");
  glBindAttribLocation(m_program, 2, "normal");
  glBindAttribLocation(m_program, 3, "colorCoord");

  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
}

std::string Shader_line::shaderFileName(){
  return "lineShader";
}

Shader_line::~Shader_line() {
  for (unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader_line::Bind() {
  glUseProgram(m_program);
}

void Shader_line::UnBind() {
  glUseProgramObjectARB(0);
}


void Shader_line::Update(Camera *camera, Light *light, std::vector<float>& clr) {
  glm::mat4 MVP;
  camera->GetVP(MVP);
  glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
  glUniform3f(glGetUniformLocation(m_program, "colorMono"), clr[0], clr[1], clr[2]);

}


} // rai_graphics