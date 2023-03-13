#include "struct_collection.h"


Collection::Collection(){
  //---------------------------

  //IDs
  this->ID_col_perma = 0;
  this->ID_col_order = 0;
  this->ID_obj_selected = 0;
  this->ID_obj_last = 0;
  this->ID_obj_otf = 0;

  //Info
  this->name = "";
  this->file_format = "";
  this->path_file_load = "";
  this->path_file_save = "";
  this->lidar_model = "";

  this->is_visible = true;
  this->is_heatmap = false;
  this->is_boxed = false;
  this->is_onthefly = false;

  //Object
  this->nb_obj = 0;
  this->selected_obj = nullptr;

  //---------------------------
}
Collection::Collection(std::string name){
  //---------------------------

  //IDs
  this->ID_col_perma = 0;
  this->ID_col_order = 0;
  this->ID_obj_selected = 0;
  this->ID_obj_last = 0;
  this->ID_obj_otf = 0;

  //Info
  this->name = name;
  this->file_format = "";
  this->path_file_load = "";
  this->path_file_save = "";
  this->lidar_model = "";

  this->is_visible = true;
  this->is_heatmap = false;
  this->is_boxed = false;
  this->is_onthefly = false;

  //Object
  this->nb_obj = 0;
  this->selected_obj = nullptr;

  //---------------------------
}

//Misc function
void Collection::reset(){
  //---------------------------

  for(int i=0; i<list_obj.size(); i++){
    Object_* object = get_obj(i);
    Object_* object_init = get_obj_init(i);

    //Reinitialize visibility
    object->is_visible = i == 0 ? true : false;

    //Reinitialize main data
    object->xyz = object_init->xyz;
    object->rgb = object_init->rgb;
    object->Nxyz = object_init->Nxyz;

    //Reset pose
    object->root = glm::vec3(0,0,0);
    object->scale = glm::mat4(1.0);
    object->trans = glm::mat4(1.0);
    object->rotat = glm::mat4(1.0);
  }

  //---------------------------
}

//Add / remove object
void Collection::obj_add_new(Object_* object){
  //---------------------------

  //Initialize parameters
  object->is_visible = true;
  Object_* list_obj_buffer = new Object_(*object);
  Object_* list_obj_init = new Object_(*object);

  //Insert new object into cloud lists
  this->list_obj.push_back(object);
  this->list_obj_buffer.push_back(list_obj_buffer);
  this->list_obj_init.push_back(list_obj_init);

  //Update number of cloud object
  this->nb_obj = this->list_obj.size();
  this->ID_obj_selected = object->ID;
  this->selected_obj = object;

  //---------------------------
}
void Collection::obj_remove_last(){
  //---------------------------

  //Get list_obj object
  Object_* sub = this->get_obj(0);
  Object_* sub_buf = this->get_obj_buffer(0);
  Object_* sub_ini = this->get_obj_init(0);

  //Remove data from GPU
  glDeleteBuffers(1, &sub->vbo_xyz);
  glDeleteBuffers(1, &sub->vbo_rgb);
  glDeleteBuffers(1, &sub->vbo_uv);
  glDeleteVertexArrays(1, &sub->vao);

  //Supress Subset pointer
  list_obj.pop_front();
  list_obj_buffer.pop_front();
  list_obj_init.pop_front();

  //Delete Subset object
  delete sub;
  delete sub_buf;
  delete sub_ini;

  //---------------------------
  this->nb_obj = list_obj.size();
}
void Collection::obj_remove_all(){
  int size = list_obj.size();
  //---------------------------

  for(int i=0; i<size; i++){
    this->obj_remove_last();
  }

  //---------------------------
}

// Get object frame
Frame* Collection::get_frame_byID(int querry){
  //---------------------------

  Object_* object = get_obj_byID(querry);
  if(object != nullptr){
    //return object->get_frame();
  }else{
    return nullptr;
  }

  //---------------------------
}

//Retrieve object
Object_* Collection::get_obj_selected_init(){
  //---------------------------

  for(int i=0; i<list_obj.size(); i++){
    Object_* sub = *next(list_obj_init.begin(), i);

    if(sub->ID == ID_obj_selected){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj(int querry){
  //---------------------------

  if(querry > list_obj.size()) return nullptr;
  else{
    return *next(list_obj.begin(), querry);
  }

  //---------------------------
}
Object_* Collection::get_obj_byID(int querry){
  //---------------------------

  for(int i=0; i<list_obj.size(); i++){
    Object_* sub = *next(list_obj.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj_buffer(int querry){
  //---------------------------

  return *next(list_obj_buffer.begin(), querry);

  //---------------------------
}
Object_* Collection::get_obj_buffer_byID(int querry){
  //---------------------------

  for(int i=0; i<list_obj.size(); i++){
    Object_* sub = *next(list_obj_buffer.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
Object_* Collection::get_obj_init(int querry){
  //---------------------------

  if(querry > list_obj_init.size()) return nullptr;
  else{
    return *next(list_obj_init.begin(), querry);
  }

  //---------------------------
}
Object_* Collection::get_obj_init_byID(int querry){
  //---------------------------

  for(int i=0; i<list_obj.size(); i++){
    Object_* sub = *next(list_obj_init.begin(), i);

    if(sub->ID == querry){
      return sub;
    }
  }

  //---------------------------
  return nullptr;
}
