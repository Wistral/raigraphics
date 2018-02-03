#include "raiGraphics/imp/shader_basic.h"
#include <iostream>
#include <fstream>

namespace rai_graphics {

Shader_basic::Shader_basic() {
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

std::string Shader_basic::shaderFileName() {
  return "basicShader";
}

Shader_basic::~Shader_basic() {
  for (unsigned int i = 0; i < NUM_SHADERS; i++) {
    glDetachShader(m_program, m_shaders[i]);
    glDeleteShader(m_shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader_basic::Bind() {
  glUseProgram(m_program);
}

void Shader_basic::UnBind() {
  glUseProgramObjectARB(0);
}

void Shader_basic::Update(Camera *camera, Light *light, object::SingleBodyObject *obj) {
  glDisable(GL_CLIP_DISTANCE0);
  glDisable(GL_CLIP_DISTANCE1);
  glDisable(GL_CLIP_DISTANCE2);
  glDisable(GL_CLIP_DISTANCE3);
  glDisable(GL_CLIP_DISTANCE4);

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

  glm::vec4 clipingPlane0(0, 0, -1, 1e5);
  glm::vec4 clipingPlane1(0, 0, -1, 1e5);
  glm::vec4 clipingPlane2(0, 0, -1, 1e5);
  glm::vec4 clipingPlane3(0, 0, -1, 1e5);
  glm::vec4 clipingPlane4(0, 0, -1, 1e5);

  glCullFace(GL_BACK);

  camera->GetVP(MVP);
  glm::vec3 CamPos;
  camera->GetPos(CamPos);

  glm::vec4 posl_g(posl[0], posl[1], posl[2], 1);
  glm::vec4 lightInCamFrame = zflip * posl_g;
  MVP = MVP * zflip * trans.GetM() * scale;
  glm::mat4 Normal = zflip * trans.GetModel() * scale;
  glm::vec4 test;

  glUniform4f(glGetUniformLocation(m_program, "clipingPlane0"), clipingPlane0[0], clipingPlane0[1], clipingPlane0[2], clipingPlane0[3]);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane1"), clipingPlane1[0], clipingPlane1[1], clipingPlane1[2], clipingPlane1[3]);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane2"), clipingPlane2[0], clipingPlane2[1], clipingPlane2[2], clipingPlane2[3]);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane3"), clipingPlane3[0], clipingPlane3[1], clipingPlane3[2], clipingPlane3[3]);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane4"), clipingPlane4[0], clipingPlane4[1], clipingPlane4[2], clipingPlane4[3]);

  glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(m_program, "Normal"), 1, GL_FALSE, &Normal[0][0]);
  glUniform3f(glGetUniformLocation(m_program, "cameraPos"), CamPos.x, CamPos.y, CamPos.z);
  glUniform3f(glGetUniformLocation(m_program, "colorMono"), clr[0], clr[1], clr[2]);
  glUniform3f(glGetUniformLocation(m_program, "lightPos"), lightInCamFrame[0], lightInCamFrame[1], lightInCamFrame[2]);
  glUniform3f(glGetUniformLocation(m_program, "mambient"), amb[0], amb[1], amb[2]);
  glUniform3f(glGetUniformLocation(m_program, "mdiffuse"), diff[0], diff[1], diff[2]);
  glUniform3f(glGetUniformLocation(m_program, "mspecular"), spec[0], spec[1], spec[2]);
  glUniform3f(glGetUniformLocation(m_program, "lambient"), ambl[0], ambl[1], ambl[2]);
  glUniform3f(glGetUniformLocation(m_program, "ldiffuse"), diffl[0], diffl[1], diffl[2]);
  glUniform3f(glGetUniformLocation(m_program, "lspecular"), specl[0], specl[1], specl[2]);
  glUniform1f(glGetUniformLocation(m_program, "shininess"), shine);    //shininess
  glUniform1f(glGetUniformLocation(m_program, "transparency"), obj->getTransparency());
  glUniform1f(glGetUniformLocation(m_program, "hasColorCoord"), obj->hasColorCoord);

}

void Shader_basic::UpdateForReflection(Camera *camera, Light *light, object::SingleBodyObject *obj, object::CheckerBoard *chk) {
  glEnable(GL_CLIP_DISTANCE0);
  glEnable(GL_CLIP_DISTANCE1);
  glEnable(GL_CLIP_DISTANCE2);
  glEnable(GL_CLIP_DISTANCE3);
  glEnable(GL_CLIP_DISTANCE4);
  glCullFace(GL_FRONT);

  glm::vec3 clipingPlane0(0, 0, -1);
  glm::vec3 clipingPlane1;
  glm::vec3 clipingPlane2;
  glm::vec3 clipingPlane3;
  glm::vec3 clipingPlane4;

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
  zflip[2][2] = -1;

  camera->GetVP(MVP);
  glm::vec3 CamPos;
  camera->GetPos(CamPos);

  clipingPlane1 = glm::cross(glm::vec3(chk->width/2, -chk->length/2,0)-CamPos, glm::vec3(-chk->width/2, 0, 0));
  clipingPlane2 = glm::cross(glm::vec3(chk->width/2, chk->length/2,0)-CamPos, glm::vec3(0, -chk->width/2, 0));
  clipingPlane3 = glm::cross(glm::vec3(-chk->width/2, chk->length/2,0)-CamPos, glm::vec3(chk->width/2, 0, 0));
  clipingPlane4 = glm::cross(glm::vec3(-chk->width/2, -chk->length/2,0)-CamPos, glm::vec3(0, chk->width/2, 0));

  float clip0 = 0;
  float clip1 = glm::dot(clipingPlane1, glm::vec3(chk->width/2, -chk->length/2,0));
  float clip2 = glm::dot(clipingPlane2, glm::vec3(chk->width/2, chk->length/2,0));
  float clip3 = glm::dot(clipingPlane3, glm::vec3(-chk->width/2, chk->length/2,0));
  float clip4 = glm::dot(clipingPlane4, glm::vec3(-chk->width/2, -chk->length/2,0));

  glm::vec4 posl_g(posl[0], posl[1], posl[2], 1);
  glm::vec4 lightInCamFrame = zflip * posl_g;
  MVP = MVP * zflip * trans.GetM() * scale;
  glm::mat4 Normal = zflip * trans.GetModel() * scale;
  glm::vec4 test;

  glUniform4f(glGetUniformLocation(m_program, "clipingPlane0"), clipingPlane0[0], clipingPlane0[1], clipingPlane0[2], -clip0);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane1"), clipingPlane1[0], clipingPlane1[1], clipingPlane1[2], -clip1);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane2"), clipingPlane2[0], clipingPlane2[1], clipingPlane2[2], -clip2);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane3"), clipingPlane3[0], clipingPlane3[1], clipingPlane3[2], -clip3);
  glUniform4f(glGetUniformLocation(m_program, "clipingPlane4"), clipingPlane4[0], clipingPlane4[1], clipingPlane4[2], -clip4);

  glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(m_program, "Normal"), 1, GL_FALSE, &Normal[0][0]);
  glUniform3f(glGetUniformLocation(m_program, "cameraPos"), CamPos.x, CamPos.y, CamPos.z);
  glUniform3f(glGetUniformLocation(m_program, "colorMono"), clr[0], clr[1], clr[2]);
  glUniform3f(glGetUniformLocation(m_program, "lightPos"), lightInCamFrame[0], lightInCamFrame[1], lightInCamFrame[2]);
  glUniform3f(glGetUniformLocation(m_program, "mambient"), amb[0], amb[1], amb[2]);
  glUniform3f(glGetUniformLocation(m_program, "mdiffuse"), diff[0], diff[1], diff[2]);
  glUniform3f(glGetUniformLocation(m_program, "mspecular"), spec[0], spec[1], spec[2]);
  glUniform3f(glGetUniformLocation(m_program, "lambient"), ambl[0], ambl[1], ambl[2]);
  glUniform3f(glGetUniformLocation(m_program, "ldiffuse"), diffl[0], diffl[1], diffl[2]);
  glUniform3f(glGetUniformLocation(m_program, "lspecular"), specl[0], specl[1], specl[2]);
  glUniform1f(glGetUniformLocation(m_program, "shininess"), shine);    //shininess
  glUniform1f(glGetUniformLocation(m_program, "transparency"), obj->getTransparency());

}

} // rai_graphics