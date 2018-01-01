//
// Created by jhwangbo on 17. 4. 30.
//

#include "raiGraphics/imp/shader.hpp"

#include "raiCommon/rai_utils.hpp"

namespace rai_graphics {

std::string Shader::LoadShader(const std::string &fileName) {
  std::ifstream file;
  file.open((fileName).c_str());

  std::string output;
  std::string line;

  if (file.is_open()) {
    while (file.good()) {
      getline(file, line);
      output.append(line + "\n");
    }
  } else {
    RAIFATAL("Unable to load shader: " << fileName);
  }

  return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage) {
  GLint success = 0;
  GLchar error[1024] = {0};

  if (isProgram)
    glGetProgramiv(shader, flag, &success);
  else
    glGetShaderiv(shader, flag, &success);

  if (success == GL_FALSE) {
    if (isProgram)
      glGetProgramInfoLog(shader, sizeof(error), NULL, error);
    else
      glGetShaderInfoLog(shader, sizeof(error), NULL, error);

    RAIFATAL(errorMessage << ": '" << error << "'");
  }
}

GLuint Shader::CreateShader(const std::string &text, unsigned int type) {
  GLuint shader = glCreateShader(type);

  RAIFATAL_IF(shader == 0, "Error compiling shader type " << type);

  const GLchar *p[1];
  p[0] = text.c_str();
  GLint lengths[1];
  lengths[0] = text.length();

  glShaderSource(shader, 1, p, lengths);
  glCompileShader(shader);

  CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

  return shader;
}

} // rai_graphics
