#ifndef CLOUD_STRUCT_H
#define CLOUD_STRUCT_H

#include "struct_subset.h"
#include "../../Module/SLAM/struct_voxelMap.h"

#include <string>
#include <vector>


struct Cloud{ //Global set of the cloud
  GLuint ID; //Permanent cloud ID
  GLuint oID; // List order cloud ID

  //Parameters
  int nb_subset;
  int nb_point;
  int subset_selected;
  int point_size;
  bool visibility;
  bool heatmap;

  //Infos
  std::string path;
  std::string name;
  std::string format;
  std::string saveas;

  //Transformation
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::vec4 unicolor;

  //Data
  std::vector<Subset> subset;
  std::vector<Subset> subset_voxel;
  std::vector<Subset> subset_buffer;
  std::vector<Subset> subset_init;
};

#endif
