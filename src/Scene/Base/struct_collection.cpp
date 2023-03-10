#include "struct_collection.h"


Collection::Collection(){
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

//Add / remove subset
void Collection::add_new_subset(Cloud* subset){
  //---------------------------

  //Initialize parameters
  subset->is_visible = true;
  Cloud* subset_buffer = new Cloud(*subset);
  Cloud* subset_init = new Cloud(*subset);

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
void Collection::remove_subset_last(){
  //---------------------------

  //Get subset object
  Cloud* sub = this->get_subset(0);
  Cloud* sub_buf = this->get_subset_buffer(0);
  Cloud* sub_ini = this->get_subset_init(0);

  //Remove data from GPU
  glDeleteBuffers(1, &sub->vbo_xyz);
  glDeleteBuffers(1, &sub->vbo_rgb);
  glDeleteBuffers(1, &sub->vbo_uv);
  glDeleteVertexArrays(1, &sub->vao);

  //Supress Subset pointer
  subset.pop_front();
  subset_buffer.pop_front();
  subset_init.pop_front();

  //Delete Subset object
  delete sub;
  delete sub_buf;
  delete sub_ini;

  //---------------------------
  this->nb_subset = subset.size();
}
void Collection::remove_subset_all(){
  int size = subset.size();
  //---------------------------

  for(int i=0; i<size; i++){
    this->remove_subset_last();
  }

  //---------------------------
}

// Get subset frame
Frame* Collection::get_frame_selected(){
  //---------------------------

  return subset_selected->get_frame();

  //---------------------------
}
Frame* Collection::get_frame_byID(int querry){
  //---------------------------

  Cloud* subset = get_subset_byID(querry);
  if(subset != nullptr){
    return subset->get_frame();
  }else{
    return nullptr;
  }

  //---------------------------
}

//Retrieve subset
Cloud* Collection::get_subset_selected_init(){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Cloud* sub = *next(subset_init.begin(), i);

    if(sub->ID == ID_selected){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Cloud* Collection::get_subset(int querry){
  //---------------------------

  if(querry > subset.size()) return nullptr;
  else{
    return *next(subset.begin(), querry);
  }

  //---------------------------
}
Cloud* Collection::get_subset_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Cloud* sub = *next(subset.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Cloud* Collection::get_subset_buffer(int querry){
  //---------------------------

  return *next(subset_buffer.begin(), querry);

  //---------------------------
}
Cloud* Collection::get_subset_buffer_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Cloud* sub = *next(subset_buffer.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Cloud* Collection::get_subset_init(int querry){
  //---------------------------

  if(querry > subset_init.size()) return nullptr;
  else{
    return *next(subset_init.begin(), querry);
  }

  //---------------------------
}
Cloud* Collection::get_subset_init_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Cloud* sub = *next(subset_init.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
