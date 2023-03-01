#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "../../common.h"


class Object_base
{
public:
  //Constructor / Destructor
  Object_base();
  ~Object_base();

public:

protected:
  //IDs
  GLuint ID_perm; //Permanent ID
  GLuint ID_order; // List order ID

  std::vector<glm::vec3> xyz;
  std::vector<glm::vec4> RGB;
  std::vector<glm::vec3> N;

  //Infos
  std::string name;
  std::string format;
  std::string path_file;
  std::string path_save;
  std::string draw_type;
  std::string dataFormat;
  bool visibility;
  int nb_point;

  //Pose
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::vec4 unicolor;
  glm::mat4 rotat;
  glm::mat4 trans;
  glm::mat4 scale;
  glm::mat4 transformation;
  Eigen::Matrix3d pose_R;
  Eigen::Vector3d pose_T;

  bool has_color;
  bool has_normal;



  //OpenGL stuff
  GLuint VAO;
  GLuint VBO_xyz;
  GLuint VBO_rgb;
  GLuint VBO_N;



};

#endif
