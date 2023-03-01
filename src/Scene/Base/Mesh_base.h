#ifndef MESH_BASE_H
#define MESH_BASE_H

#include "../../common.h"


class Mesh_base
{
public:
  //Constructor / Destructor
  Mesh_base();
  ~Mesh_base();

public:

private:
  //IDs
  GLuint ID; //Permanent ID
  GLuint oID; // List order ID

  //Infos
  std::string path;
  std::string name;
  std::string format;
  std::string saveas;
  std::string draw_type;
  std::string dataFormat;
  bool visibility;
  int nb_point;

  //Transformation
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::vec4 unicolor;

};

#endif
