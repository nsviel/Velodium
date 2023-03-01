#ifndef CLOUD_BASE_H
#define CLOUD_BASE_H

#include "Object_base.h"

#include <list>


class Cloud_base : public Object_base
{
public:
  //Constructor / Destructor
  Cloud_base();
  ~Cloud_base();

public:
  void init_cloud();
  void reset_cloud();
  Subset* get_subset_byID(int ID);

public:
  //Cloud data
  std::vector<glm::vec3> xyz_voxel;
  std::vector<float> I;
  std::vector<float> ts;
  std::vector<double> ts_n;
  std::vector<float> A;
  std::vector<float> R;
  std::vector<float> cosIt;
  std::vector<float> It;
  std::vector<int> selected;
  std::list<int> highlighted;

  bool has_intensity;
  bool has_timestamp;

  //Cloud info
  int nb_subset;
  int nb_point;
  int point_size;
  std::string lidar_model;

  bool is_heatmap;
  bool is_onthefly;
  bool is_boxed;

  //Cloud subset
  int ID_sub_selected; //Actual selected subset ID
  int ID_sub_last_given; //Last given subset ID
  int ID_sub_cur_file; //For on-the-fly loading

  Cloud_base* subset_selected;
  std::list<Cloud_base*> subset;
  std::list<Cloud_base*> subset_voxel;
  std::list<Cloud_base*> subset_buffer;
  std::list<Cloud_base*> subset_init;
  std::vector<std::string> list_path;
  std::list<int> list_loaded;
};

#endif
