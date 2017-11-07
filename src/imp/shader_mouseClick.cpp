#include "raiGraphics/imp/shader_mouseClick.h"
#include <iostream>
#include <fstream>
#include <glog/logging.h>

namespace rai_graphics {

Shader_mouseClick::Shader_mouseClick() {
  m_program = glCreateProgram();
  m_shaders[0] = CreateShader(LoadShader(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/" + shaderFileName() + ".vs"), GL_VERTEX_SHADER);
  m_shaders[1] = CreateShader(LoadShader(std::string(getenv("RAI_GRAPHICS_OPENGL_ROOT")) + "/res/" + shaderFileName() + ".fs"), GL_FRAGMENT_SHADER);

  for (unsigned int i = 0; i < NUM_SHADERS; i++)
    glAttachShader(m_program, m_shaders[i]);

  glBindAttribLocation(m_program, 0, "position");

  glLinkProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

  glValidateProgram(m_program);
  CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
}

std::string Shader_mouseClick::shaderFileName(){
  return "mouseClickShader";
}

Shader_mouseClick::~Shader_mouseClick() {
  for (unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader_mouseClick::Bind() {
  glUseProgram(m_program);
}

void Shader_mouseClick::UnBind() {
  glUseProgramObjectARB(0);
}

void Shader_mouseClick::Update(Camera *camera, Light *light, object::SingleBodyObject* obj, bool isreflection) {
  LOG(FATAL)<<"unused method";
}

void Shader_mouseClick::Update(Camera *camera, object::SingleBodyObject* obj) {
  Transform trans;
  std::vector<float> clr;
  obj->getTransform(trans);
  glm::mat4 scale = obj->getScale();
  glm::mat4 MVP;
  glCullFace(GL_BACK);

  camera->GetVP(MVP);
  glm::vec3 CamPos;
  camera->GetPos(CamPos);
  MVP = MVP * trans.GetM() * scale;

  glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
  glUniform3f(glGetUniformLocation(m_program, "color0"),(float)(obj->getObIndex()%255)/255,
              (float)((obj->getObIndex()/255)%255)/255, (float)((obj->getObIndex()/255/255)%255)/255);
}

} // rai_graphics