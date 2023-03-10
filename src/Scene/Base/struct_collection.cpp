#include "struct_collection.h"


Collection::Collection(){
  //---------------------------

  //IDs
  this->ID_col_perma = 0;
  this->ID_col_order = 0;

  //Info
  this->obj_type = "collection";
  this->lidar_model = "";
  this->is_visible = true;
  this->is_heatmap = false;
  this->is_boxed = false;

  //Onthefly
  this->is_onthefly = false;
  this->ID_onthefly = 0;

  //Subset
  this->ID_obj_selected = 0;
  this->ID_obj_last = 0;
  this->nb_obj = 0;
  this->selected_obj = nullptr;

  //---------------------------
}

//Add / remove subset
void Collection::add_new_subset(Object_* subset){
  //---------------------------

  //Initialize parameters
  subset->is_visible = true;
  Object_* subset_buffer = new Object_(*subset);
  Object_* subset_init = new Object_(*subset);

  //Insert new subset into cloud lists
  this->subset.push_back(subset);
  this->subset_buffer.push_back(subset_buffer);
  this->subset_init.push_back(subset_init);

  //Update number of cloud subset
  this->nb_obj = this->subset.size();
  this->ID_obj_selected = subset->ID;
  this->selected_obj = subset;

  //---------------------------
}
void Collection::remove_subset_last(){
  //---------------------------

  //Get subset object
  Object_* sub = this->get_obj(0);
  Object_* sub_buf = this->get_obj_buffer(0);
  Object_* sub_ini = this->get_obj_init(0);

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
  this->nb_obj = subset.size();
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
Frame* Collection::get_frame_byID(int querry){
  //---------------------------

  Object_* subset = get_obj_byID(querry);
  if(subset != nullptr){
    //return subset->get_frame();
  }else{
    return nullptr;
  }

  //---------------------------
}

//Retrieve subset
Object_* Collection::get_obj_selected_init(){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Object_* sub = *next(subset_init.begin(), i);

    if(sub->ID == ID_obj_selected){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj(int querry){
  //---------------------------

  if(querry > subset.size()) return nullptr;
  else{
    return *next(subset.begin(), querry);
  }

  //---------------------------
}
Object_* Collection::get_obj_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Object_* sub = *next(subset.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj_buffer(int querry){
  //---------------------------

  return *next(subset_buffer.begin(), querry);

  //---------------------------
}
Object_* Collection::get_obj_buffer_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Object_* sub = *next(subset_buffer.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj_init(int querry){
  //---------------------------

  if(querry > subset_init.size()) return nullptr;
  else{
    return *next(subset_init.begin(), querry);
  }

  //---------------------------
}
Object_* Collection::get_obj_init_byID(int querry){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Object_* sub = *next(subset_init.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
