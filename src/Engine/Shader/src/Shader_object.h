#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "../../../common.h"


class Shader_object
{
public:
  Shader_object(string vertex, string fragment);
  Shader_object(string vertex, string fragment, string path_gs);
  ~Shader_object();

public:
  void use();
  void build_shader(GLuint& program_ID, string path_vs, string path_fs);

  inline void setBool(const std::string &name, bool value){glUniform1i(glGetUniformLocation(program_ID, name.c_str()), (int)value);}
  inline void setInt(const std::string &name, int value){glUniform1i(glGetUniformLocation(program_ID, name.c_str()), value);}
  inline void setFloat(const std::string &name, float value){glUniform1f(glGetUniformLocation(program_ID, name.c_str()), value);}
  inline void setVec2(const std::string &name, glm::vec2 &value){glUniform2fv(glGetUniformLocation(program_ID, name.c_str()), 1, &value[0]);}
  inline void setVec2(const std::string &name, float x, float y){glUniform2f(glGetUniformLocation(program_ID, name.c_str()), x, y);}
  inline void setVec3(const std::string &name, glm::vec3 &value){glUniform3fv(glGetUniformLocation(program_ID, name.c_str()), 1, &value[0]);}
  inline void setVec3(const std::string &name, float x, float y, float z){glUniform3f(glGetUniformLocation(program_ID, name.c_str()), x, y, z);}
  inline void setVec4(const std::string &name, glm::vec4 &value){glUniform4fv(glGetUniformLocation(program_ID, name.c_str()), 1, &value[0]);}
  inline void setVec4(const std::string &name, float x, float y, float z, float w){glUniform4f(glGetUniformLocation(program_ID, name.c_str()), x, y, z, w);}
  inline void setMat2(const std::string &name, glm::mat2 &mat){glUniformMatrix2fv(glGetUniformLocation(program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}
  inline void setMat3(const std::string &name, glm::mat3 &mat){glUniformMatrix3fv(glGetUniformLocation(program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}
  inline void setMat4(const std::string &name, glm::mat4 &mat){glUniformMatrix4fv(glGetUniformLocation(program_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);}

  inline GLuint get_program_ID(){return program_ID;}

private:
  GLuint shader_compilation(string fileName, GLenum shaderType);

private:
  GLuint program_ID;
};

#endif
