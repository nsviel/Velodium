#ifndef SUBSET_STRUCT_H
#define SUBSET_STRUCT_H

#include "struct_glyph.h"
#include "struct_frame.h"
#include "struct_obstacle.h"
#include "struct_register.h"

#include <string>
#include <vector>
#include <list>


struct Subset{ //Cloud subset / part
  //---------------------------

  GLuint VAO;
  GLuint VBO_xyz;
  GLuint VBO_rgb;
  GLuint VBO_N;

  //Infos
  int ID;
  int nb_point;
  std::string name;
  std::string path;
  glm::vec4 unicolor;

  bool visibility;
  bool has_color;
  bool has_intensity;
  bool has_normal;
  bool has_timestamp;

  //Main data
  std::vector<glm::vec3> xyz;
  std::vector<glm::vec3> xyz_voxel;
  std::vector<glm::vec4> RGB;
  std::vector<glm::vec3> N;
  std::vector<float> I;

  std::vector<float> ts;
  std::vector<double> ts_n;
  float angle;

  //Various attributs
  std::vector<float> A;
  std::vector<float> R;
  std::vector<float> cosIt;
  std::vector<float> It;
  std::vector<int> selected;
  std::list<int> highlighted;

  //Pose
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::mat4 rotat;
  glm::mat4 trans;
  glm::mat4 scale;
  glm::mat4 transformation;

  //Own glyphs
  Glyph normal;
  Glyph axis;
  Glyph keypoint;

  //Specific structures
  Frame frame;
  Register icp;
  Obstac obstacle_gt;
  Obstac obstacle_pr;

  //---------------------------
};

#endif
