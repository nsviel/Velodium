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
Frame* Cloud::get_frame_byID(int querry){
  //---------------------------

  Subset* subset = get_subset_byID(querry);
  if(subset != nullptr){
    return subset->get_frame();
  }else{
    return nullptr;
  }

  //---------------------------
}

Subset* Cloud::get_subset_selected(){
  //---------------------------

  if(cloud_selected != nullptr){
    int ID_subset = cloud_selected->ID_selected;

    for(int i=0; i<cloud_selected->subset.size(); i++){
      Subset* subset = *next(cloud_selected->subset.begin(), i);
      if(ID_subset == subset->ID){
        return subset;
      }
    }

  }

  //---------------------------
  return nullptr;
}
Subset* Cloud::get_subset_selected_init(){
  Cloud* cloud = cloud_selected;
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset_init.begin(), i);

    if(subset->ID == cloud->ID_selected){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Cloud::get_subset(int querry){
  //---------------------------

  if(i > subset.size()) return nullptr;
  else{
    return *next(subset.begin(), i);
  }

  //---------------------------
}
Subset* Cloud::get_subset_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Subset* sub = *next(subset.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Graph::get_subset_buffer(int querry){
  //---------------------------

  return *next(cloud->subset_buffer.begin(), querry);

  //---------------------------
}
Subset* Graph::get_subset_buffer_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Subset* sub = *next(subset_buffer.begin(), i);

    if(subset->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Cloud::get_subset_init(int querry){
  //---------------------------

  if(querry > subset_init.size()) return nullptr;
  else{
    return *next(subset_init.begin(), querry);
  }

  //---------------------------
}
Subset* Graph::get_subset_init_byID(int querry){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* sub = *next(cloud->subset_init.begin(), i);

    if(sub->ID == querry){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
