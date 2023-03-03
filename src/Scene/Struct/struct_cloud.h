#ifndef CLOUD_STRUCT_H
#define CLOUD_STRUCT_H

#include "struct_object.h"
#include "struct_subset.h"

#include <string>
#include <list>


struct Cloud : public Object_
{
  //---------------------------

  Cloud();
  Subset* get_subset_byID(int querry_ID);

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
