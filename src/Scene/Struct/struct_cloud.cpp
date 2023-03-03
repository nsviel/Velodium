#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  //IDs
  this->ID_perma = 0;
  this->ID_order = 0;

  //Info
  this->lidar_model = "";
  this->is_visible = true;
  this->is_heatmap = false;
  this->is_boxed = false;

  //Onthefly
  this->is_onthefly = false;
  this->ID_onthefly = 0;

  //Subset
  this->ID_selected = 0;
  this->ID_subset = 0;
  this->nb_subset = 0;
  this->subset_selected = nullptr;

  //---------------------------
}

Subset* Cloud::get_subset_byID(int querry_ID){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Subset* sub = *next(subset.begin(), i);

    if(sub->ID == querry_ID){
      return sub;
    }
  }

  //---------------------------
  return subset_selected;
}
