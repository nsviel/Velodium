#ifndef SCENE_H
#define SCENE_H

#include "Data.h"
#include "../../common.h"

#include <list>
#include <GLFW/glfw3.h>

class Object;
class GPU_data;


class Scene
{
public:
  //Constructor / Destructor
  Scene();
  ~Scene();

public:
  //Remove functions
  void remove_collection(Collection* collection);
  void remove_cloud_all();

  //Reset functions
  void reset_collection(Collection* collection);
  void reset_collection_all();

  //Update function
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void update_glyph(Object_* object);
  void update_cloud_IntensityToColor(Collection* collection);
  void update_subset_IntensityToColor(Cloud* cloud);
  void update_col_MinMax(Collection* collection);
  void update_MinMax(Object_* object);
  void update_ID_order(list<Collection*>* list);

  inline int get_nb_cloud(){return data->get_list_collection()->size();}
  inline list<Collection*>* get_list_collection(){return data->get_list_collection();}
  inline Collection* get_selected_collection(){return data->get_cloud_selected();}
  inline void set_selected_col(Collection* col){}
  inline bool get_is_list_empty(){return data->get_list_collection()->empty();}

private:
  Object* objectManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
