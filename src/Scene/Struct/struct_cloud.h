#ifndef CLOUD_STRUCT_H
#define CLOUD_STRUCT_H

#include "struct_object.h"
#include "struct_subset.h"

#include <string>
#include <list>


//Cloud are subset containers
struct Cloud : public Object_
{
  //---------------------------

  Cloud();

  void add_new_subset(Subset* subset);

  Frame* get_frame_byID(int querry);
  Frame* get_frame_selected();

  Subset* get_subset_selected_init();
  Subset* get_subset(int querry);
  Subset* get_subset_byID(int querry);
  Subset* get_subset_buffer(int querry);
  Subset* get_subset_buffer_byID(int querry);
  Subset* get_subset_init(int querry);
  Subset* get_subset_init_byID(int querry);

  //IDs
  int ID_perma; //Permanent cloud ID
  int ID_order; // List order cloud ID

  //Infos
  std::string lidar_model;
  bool is_heatmap;
  bool is_boxed;

  //On the fly
  bool is_onthefly;
  int ID_onthefly;
  std::vector<std::string> list_path;
  std::list<int> list_loaded;

  //Subset stuff
  int ID_selected; //Actual selected subset ID
  int ID_subset; //Last given subset ID
  int nb_subset;
  Subset* subset_selected;
  std::list<Subset*> subset;
  std::list<Subset*> subset_voxel;
  std::list<Subset*> subset_buffer;
  std::list<Subset*> subset_init;

  //---------------------------
};

#endif
