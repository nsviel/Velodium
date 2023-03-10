#ifndef COLLECTION_STRUCT_H
#define COLLECTION_STRUCT_H

#include "struct_object.h"
#include "struct_cloud.h"

#include <string>
#include <list>


//Collection are subset containers
struct Collection
{
  //---------------------------

  //Add / remove functions  A REVOIR
  Collection();
  void add_new_subset(Object_* subset);
  void remove_subset_all();
  void remove_subset_last();
  Frame* get_frame_byID(int querry);

  //Stored object retrieval function
  Object_* get_obj_selected_init();
  Object_* get_obj(int querry);
  Object_* get_obj_byID(int querry);
  Object_* get_obj_buffer(int querry);
  Object_* get_obj_buffer_byID(int querry);
  Object_* get_obj_init(int querry);
  Object_* get_obj_init_byID(int querry);

  //IDs
  int ID_col_perma; //Permanent collection ID
  int ID_col_order; // List order collection ID
  int ID_obj_selected; //Actual selected object ID
  int ID_obj_last; //Last given object ID

  //Infos   A REVOIR
  std::string lidar_model;
  std::string name;
  std::string path_file;
  std::string path_save;
  int nb_point;
  std::string file_format;
  bool is_heatmap;
  bool is_boxed;
  bool is_visible;
  int draw_point_size;
  glm::vec4 unicolor;
  std::string obj_type;

  //Stored Objects  A REVOIR
  int nb_object;
  Object_* subset_selected;
  std::list<Object_*> subset;
  std::list<Object_*> subset_voxel;
  std::list<Object_*> subset_buffer;
  std::list<Object_*> subset_init;

  //On-the-fly  A REVOIR
  bool is_onthefly;
  int ID_onthefly;
  std::vector<std::string> list_path;
  std::list<int> list_loaded;
  glm::vec3 COM;
  glm::vec3 min;
  glm::vec3 max;

  //---------------------------
};

#endif
