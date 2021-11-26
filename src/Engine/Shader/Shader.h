#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

#include <fstream>


class Shader
{
public:
  Shader();
  ~Shader();

public:
  bool shader_build(string name);
  void run();

  void setBool(const std::string &name, bool value) const{
      glUniform1i(glGetUniformLocation(ID_program, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const{
      glUniform1i(glGetUniformLocation(ID_program, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const{
      glUniform1f(glGetUniformLocation(ID_program, name.c_str()), value);
  }
  void setVec2(const std::string &name, const glm::vec2 &value) const{
      glUniform2fv(glGetUniformLocation(ID_program, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string &name, float x, float y) const{
      glUniform2f(glGetUniformLocation(ID_program, name.c_str()), x, y);
  }
  void setVec3(const std::string &name, const glm::vec3 &value) const{
      glUniform3fv(glGetUniformLocation(ID_program, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const{
      glUniform3f(glGetUniformLocation(ID_program, name.c_str()), x, y, z);
  }
  void setVec4(const std::string &name, const glm::vec4 &value) const{
      glUniform4fv(glGetUniformLocation(ID_program, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z, float w){
      glUniform4f(glGetUniformLocation(ID_program, name.c_str()), x, y, z, w);
  }
  void setMat2(const std::string &name, const glm::mat2 &mat) const{
      glUniformMatrix2fv(glGetUniformLocation(ID_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat3(const std::string &name, const glm::mat3 &mat) const{
      glUniformMatrix3fv(glGetUniformLocation(ID_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat4(const std::string &name, const glm::mat4 &mat) const{
      glUniformMatrix4fv(glGetUniformLocation(ID_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

private:
  GLuint shader_compilation(string fileName, GLenum shaderType);

private:
  uint ID_program;
  string shader_dir;
};

#endif
