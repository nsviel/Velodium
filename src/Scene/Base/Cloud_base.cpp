#include "Cloud_base.h"


//Constructor / Destructor
Cloud_base::Cloud_base(){
  //---------------------------

  this->init_cloud();

  //---------------------------
}
Cloud_base::~Cloud_base(){}

//Main function
void Cloud_base::init_cloud(){
  //---------------------------

  this->init_object();

  this->has_intensity = false;
  this->has_timestamp = false;

  this->nb_subset = 0;
  this->nb_point = 0;
  this->point_size = 1;
  this->lidar_model = "";

  this->is_heatmap = false;
  this->is_onthefly = false;
  this->is_boxed = false;

  this->ID_sub_selected = 0;
  this->ID_sub_last_given = 0;
  this->ID_sub_cur_file = 0;

  this->subset_selected = nullptr;

  //---------------------------
}
void Cloud_base::reset_cloud(){
  //---------------------------

    this->reset_object();

  //---------------------------
}
Subset* Cloud_base::get_subset_byID(int ID){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Subset* sub = *next(subset.begin(), i);

    if(sub->ID == ID){
      return sub;
    }
  }

  //---------------------------
  return subset_selected;
}
