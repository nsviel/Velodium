#ifndef SCENE_H
#define SCENE_H

#include "Data.h"
#include "../../common.h"

#include <list>
#include <GLFW/glfw3.h>

class Node_scene;
class Object;
class GPU_data;


class Scene
{
public:
  //Constructor / Destructor
  Scene(Node_scene* node);
  ~Scene();

public:
  //Remove functions
  void remove_cloud(Cloud* cloud);
  void remove_cloud_all();

  //Reset functions
  void reset_cloud(Cloud* cloud);
  void reset_cloud_all();

  //Update function
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void update_glyph(Object_* object);
  void update_cloud_IntensityToColor(Cloud* cloud);
  void update_subset_IntensityToColor(Subset* subset);
  void update_cloud_MinMax(Cloud* cloud);
  void update_MinMax(Object_* object);
  void update_ID_order(list<Cloud*>* list);

  inline int get_nb_cloud(){return data->get_list_cloud()->size();}
  inline list<Cloud*>* get_list_cloud(){return data->get_list_cloud();}
  inline Cloud* get_selected_cloud(){return cloud_selected;}
  inline void set_selected_cloud(Cloud* cloud){cloud_selected = cloud;}
  inline bool get_is_list_empty(){return data->get_list_cloud()->empty();}
  inline int get_new_oID_cloud(){return data->get_list_cloud()->size();}
  inline Data* get_data(){return data;}

private:
  Object* objectManager;
  GPU_data* gpuManager;
  Data* data;

  Cloud* cloud_selected;
};

#endif
