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
  void remove_cloud(Collection* cloud);
  void remove_cloud_all();

  //Reset functions
  void reset_cloud(Collection* cloud);
  void reset_cloud_all();

  //Update function
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void update_glyph(Object_* object);
  void update_cloud_IntensityToColor(Collection* cloud);
  void update_subset_IntensityToColor(Subset* subset);
  void update_cloud_MinMax(Collection* cloud);
  void update_MinMax(Object_* object);
  void update_ID_order(list<Collection*>* list);

  inline int get_nb_cloud(){return data->get_list_cloud()->size();}
  inline list<Collection*>* get_list_cloud(){return data->get_list_cloud();}
  inline Collection* get_selected_cloud(){return data->get_cloud_selected();}
  inline void set_selected_cloud(Collection* cloud){Collection* cl = data->get_cloud_selected(); cl = cloud;}
  inline bool get_is_list_empty(){return data->get_list_cloud()->empty();}

private:
  Object* objectManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
