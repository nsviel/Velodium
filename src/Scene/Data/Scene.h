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
  void remove_collection_all();

  //Reset functions
  void reset_collection(Collection* collection);
  void reset_collection_all();

  //Update collection
  void update_collection_location(Collection* collection);
  void update_collection_color(Collection* collection);
  void update_collection_MinMax(Collection* collection);

  //Update object function
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void update_MinMax(Object_* object);

  //Update collection function
  void update_glyph(Collection* collection);
  void update_MinMax_col(Collection* collection);

  inline int get_nb_cloud(){return data->get_list_col_object()->size();}
  inline list<Collection*>* get_list_col_object(){return data->get_list_col_object();}
  inline Collection* get_selected_collection(){return data->get_selected_collection();}
  inline void set_selected_collection(Collection* col){}
  inline bool get_is_list_empty(){return data->get_list_col_object()->empty();}

private:
  Object* objectManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
