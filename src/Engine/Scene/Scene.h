#ifndef SCENE_H
#define SCENE_H

#include "../../common.h"

#include <list>
#include <GLFW/glfw3.h>

class Data;
class Node_engine;
class Object;


class Scene
{
public:
  //Constructor / Destructor
  Scene(Node_engine* node_engine);
  ~Scene();

public:
  //Remove functions
  void remove_cloud(Cloud* cloud);
  void remove_cloud_all();
  void remove_subset(Cloud* cloud, int ID);
  void remove_subset_to_gpu(Subset* subset);
  void remove_subset_last(Cloud* cloud);
  void remove_subset_all(Cloud* cloud);

  //Add functions
  void add_new_subset(Cloud* cloud, Subset* subset);
  void add_subset_to_gpu(Subset* subset);

  //Reset functions
  void reset_cloud(Cloud* cloud);
  void reset_cloud_all();

  //Update cloud
  void update_cloud_glyphs(Cloud* cloud);
  void update_cloud_IntensityToColor(Cloud* cloud);
  void update_cloud_oID(list<Cloud*>* list);
  void update_cloud_MinMax(Cloud* cloud);
  void update_cloud_location(Cloud* cloud);
  void update_cloud_color(Cloud* cloud);
  void update_cloud_dataFormat(Cloud* cloud);

  //Update subset
  void update_subset_glyphs(Subset* subset);
  void update_subset(Subset* subset);
  void update_subset_IntensityToColor(Subset* subset);
  void update_subset_MinMax(Subset* subset);
  void update_subset_location(Subset* subset);
  void update_subset_color(Subset* subset);

  //Subfunctions
  void selection_setNext();
  void selection_setCloud(int ID);
  void selection_setSubset(Cloud* cloud, int ID);
  void selection_cloudByName(string name);
  void selection_setCloud(Cloud* cloud);

  //Assesseurs
  Cloud* get_cloud_next();
  Subset* get_subset_selected_init();
  Subset* get_subset(Cloud* cloud, int i);
  Subset* get_subset_byID(Cloud* cloud, int ID);
  Subset* get_subset_selected();
  Subset* get_subset_buffer(Cloud* cloud, int i);
  Subset* get_subset_buffer_byID(Cloud* cloud, int ID);
  Subset* get_subset_init(Cloud* cloud, int i);
  Subset* get_subset_init_byID(Cloud* cloud, int ID);
  Frame* get_frame(Cloud* cloud, int i);
  Frame* get_frame_selected();
  Frame* get_frame_byID(Cloud* cloud, int ID);
  int get_subset_oID(Cloud* cloud, Subset* subset);
  bool get_is_list_empty();

  inline int get_nb_cloud(){return list_cloud->size();}
  inline list<Cloud*>* get_list_cloud(){return list_cloud;}
  inline int* get_new_ID_cloud(){return &ID_cloud;}
  inline int get_new_oID_cloud(){return list_cloud->size();}
  inline Cloud* get_selected_cloud(){return cloud_selected;}
  inline void set_selected_cloud(Cloud* cloud){cloud_selected = cloud;}

private:
  Data* dataManager;
  Object* objectManager;

  list<Cloud*>* list_cloud;
  Cloud* cloud_selected;
  Subset* subset_selected;
  bool is_visualization;
  int ID_cloud;
};

#endif
