#ifndef CLOUD_STRUCT_H
#define CLOUD_STRUCT_H

#include "struct_subset.h"
#include "struct_voxelMap.h"

#include <string>
#include <list>


struct Cloud{ //Global set of the cloud
  //---------------------------

  //IDs
  GLuint ID; //Permanent cloud ID
  GLuint oID; // List order cloud ID

  int ID_selected; //Actual selected subset ID
  int ID_subset; //Last given subset ID
  int ID_file; //For on-the-fly loading

  //Parameters
  int nb_subset;
  int nb_point;
  int point_size;

  //Flags
  bool visibility;
  bool heatmap;
  bool onthefly;
  bool boxed;

  //Infos
  std::string path;
  std::string name;
  std::string format;
  std::string saveas;
  std::string draw_type;
  std::string dataFormat;
  std::string lidar_model;
  std::vector<std::string> list_path;
  std::list<int> list_loaded;

  //Transformation
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::vec4 unicolor;

  //Data
  Subset* subset_selected = nullptr;
  std::list<Subset*> subset;
  std::list<Subset*> subset_voxel;
  std::list<Subset*> subset_buffer;
  std::list<Subset*> subset_init;

  Subset* get_subset_byID(int ID){
    for(int i=0; i<subset.size(); i++){
      Subset* sub = *next(subset.begin(), i);

      if(sub->ID == ID){
        return sub;
      }
    }
    return subset_selected;
  }

  //---------------------------
};

#endif
