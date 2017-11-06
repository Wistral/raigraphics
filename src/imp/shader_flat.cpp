#include "raiGraphics/imp/shader_flat.h"
#include <iostream>
#include <fstream>

namespace rai_graphics {

Shader_flat::Shader_flat() {
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

std::string Shader_flat::shaderFileName(){
  return "flatShader";
}

Shader_flat::~Shader_flat() {
  for (unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader_flat::Bind() {
  glUseProgram(m_program);
}

void Shader_flat::UnBind() {
  glUseProgramObjectARB(0);
}


void Shader_flat::Update(Camera *camera, Light *light, object::SingleBodyObject* obj, bool isreflection) {
  Transform trans;
  std::vector<float> clr, amb, diff, spec, ambl, diffl, specl, posl;
  float shine;

  obj->getTransform(trans);
  glm::mat4 scale = obj->getScale();
  obj->getLightPropAmb(amb);
  obj->getLightPropDiff(diff);
  obj->getLightPropSpec(spec);
  obj->getColor(clr);
  obj->getShiness(shine);

  light->getAmbient(ambl);
  light->getDiffuse(diffl);
  light->getSpecular(specl);
  light->getPosition(posl);

  glm::mat4 MVP, zflip;
  if(isreflection){
    zflip[2][2]=-1;
    glCullFace(GL_FRONT);
  } else {
    glCullFace(GL_BACK);
  }

  camera->GetVP(MVP);
  glm::vec3 CamPos;
  camera->GetPos(CamPos);
  glm::vec4 posl_g(posl[0], posl[1], posl[2], 1);
  glm::vec4 lightInCamFrame = zflip * posl_g;
  glm::mat4 Normal = zflip * trans.GetModel() * scale;
  MVP = MVP * zflip * trans.GetM() * scale;

  glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(m_program, "Normal"), 1, GL_FALSE, &Normal[0][0]);
  glUniform3f(glGetUniformLocation(m_program, "cameraPos"), CamPos.x, CamPos.y, CamPos.z);
  glUniform3f(glGetUniformLocation(m_program,"colorMono"),clr[0],clr[1],clr[2]);
  glUniform3f(glGetUniformLocation(m_program,"lightPos"),lightInCamFrame[0],lightInCamFrame[1],lightInCamFrame[2]);
  glUniform3f(glGetUniformLocation(m_program,"mambient"),amb[0],amb[1],amb[2]);
  glUniform3f(glGetUniformLocation(m_program,"mdiffuse"),diff[0],diff[1],diff[2]);
  glUniform3f(glGetUniformLocation(m_program,"mspecular"),spec[0],spec[1],spec[2]);
  glUniform3f(glGetUniformLocation(m_program,"lambient"),ambl[0],ambl[1],ambl[2]);
  glUniform3f(glGetUniformLocation(m_program,"ldiffuse"),diffl[0],diffl[1],diffl[2]);
  glUniform3f(glGetUniformLocation(m_program,"lspecular"),specl[0],specl[1],specl[2]);
  glUniform1f(glGetUniformLocation(m_program,"shininess"),shine);    //shininess
  glUniform1f(glGetUniformLocation(m_program,"transparency"),obj->getTransparency());
}

} // rai_graphics