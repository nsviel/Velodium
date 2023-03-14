#ifndef COLLECTION_STRUCT_H
#define COLLECTION_STRUCT_H

#include "struct_object.h"
#include "struct_cloud.h"

#include <string>
#include <list>
#include <iostream>


//Collection are cloud containers
struct Collection
{
  //---------------------------

  //Add / remove functions  A REVOIR
  Collection();
  Collection(std::string name);

  void reset();

  //Add / remove function
  void obj_add_new(Object_* object);
  void obj_remove_all();
  void obj_remove_last();

  //Stored object retrieval function
  Frame* get_frame_byID(int querry);
  Object_* get_obj_selected_init();
  Object_* get_obj(int querry);
  Object_* get_obj_byID(int querry);
  Object_* get_obj_buffer(int querry);
  Object_* get_obj_buffer_byID(int querry);
  Object_* get_obj_init(int querry);
  Object_* get_obj_init_byID(int querry);

  //Specific function
  void set_visibility(bool value);

  //IDs
  int ID_col_perma; //Permanent collection ID
  int ID_col_order; // List order collection ID
  int ID_obj_selected; //Actual selected object ID
  int ID_obj_last; //Last given object ID
  int ID_obj_otf; // On the fly ID

  //Infos   A REVOIR
  std::string name;
  std::string lidar_model;
  std::string file_format;
  std::string path_file_load;
  std::string path_file_save;
  std::string obj_type;

  glm::vec4 unicolor;
  glm::vec3 COM;
  glm::vec3 min;
  glm::vec3 max;

  bool is_visible;
  bool is_heatmap;
  bool is_boxed;
  bool is_onthefly;

  //Stored Objects
  int nb_obj;
  Object_* selected_obj;
  std::list<Object_*> list_obj;
  std::list<Object_*> list_obj_buffer;
  std::list<Object_*> list_obj_init;

  //On-the-fly
  std::vector<std::string> list_otf_path;
  std::list<int> list_otf_loaded;

  //---------------------------
};

#endif
