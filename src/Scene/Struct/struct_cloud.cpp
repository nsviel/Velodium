#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  //IDs
  this->ID_perma = 0;
  this->ID_order = 0;

  //Info
  this->obj_type = "cloud";
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
void Cloud::add_new_subset(Subset* subset){
  //---------------------------

  //Initialize parameters
  subset->is_visible = true;
  Subset* subset_buffer = new Subset(*subset);
  Subset* subset_init = new Subset(*subset);

  //Insert new subset into cloud lists
  this->subset.push_back(subset);
  this->subset_buffer.push_back(subset_buffer);
  this->subset_init.push_back(subset_init);

  //Update number of cloud subset
  this->nb_subset = this->subset.size();
  this->ID_selected = subset->ID;
  this->subset_selected = subset;

  //---------------------------
}
