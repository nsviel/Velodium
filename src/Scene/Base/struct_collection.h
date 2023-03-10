#ifndef COLLECTION_STRUCT_H
#define COLLECTION_STRUCT_H

#include "struct_object.h"
#include "struct_cloud.h"

#include <string>
#include <list>


//Collection are subset containers
struct Collection : public Object_
{
  //---------------------------

  Collection();

  void add_new_subset(Cloud* subset);
  void remove_subset_all();
  void remove_subset_last();

  Frame* get_frame_byID(int querry);
  Frame* get_frame_selected();

  Cloud* get_subset_selected_init();
  Cloud* get_subset(int querry);
  Cloud* get_subset_byID(int querry);
  Cloud* get_subset_buffer(int querry);
  Cloud* get_subset_buffer_byID(int querry);
  Cloud* get_subset_init(int querry);
  Cloud* get_subset_init_byID(int querry);

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
  Cloud* subset_selected;
  std::list<Cloud*> subset;
  std::list<Cloud*> subset_voxel;
  std::list<Cloud*> subset_buffer;
  std::list<Cloud*> subset_init;

  //---------------------------
};

#endif
