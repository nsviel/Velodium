#ifndef GENERIC_STRUCT_H
#define GENERIC_STRUCT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Texture{
  //---------------------------

  GLuint ID;
  std::string Name;
  int width = 0;
  int height = 0;

  //---------------------------
};

struct PTXCloud{
  //---------------------------

  //header
  glm::vec3 rootTrans;
  glm::mat3 rootRotat;
  glm::mat4 transfMat;

  //data
  std::vector<glm::vec3> location;
  std::vector<float> intensity;
  std::vector<glm::vec4> color;

  //---------------------------
};

struct Uplet{
  //---------------------------

  GLuint pc1;
  int idx1;
  GLuint pc2;
  int idx2;

  //---------------------------
};

struct subpart{
  //---------------------------

  int ID;
  std::string name;
  std::string namePC;
  glm::vec3 COM;
  glm::vec3 minloc;
  glm::vec3 maxloc;

  //---------------------------
};

#endif
