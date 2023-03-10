#include "Scene.h"

#include "../Glyph/Object.h"
#include "../../Engine/GPU/GPU_data.h"


//Constructor / Destructor
Scene::Scene(){
  //---------------------------

  this->objectManager = new Object();
  this->gpuManager = new GPU_data();
  this->data = Data::get_instance();

  //---------------------------
}
Scene::~Scene(){
  //---------------------------



  //---------------------------
}

//Remove functions
void Scene::remove_cloud(Collection* cloud){
  std::list<Collection*>* list_collection = data->get_list_collection();
  Collection* cloud_selected = data->get_cloud_selected();
  //---------------------------

  if(!get_is_list_empty()){
    int oID = cloud->ID_col_order;
    string name =  cloud->name;
    //---------------------------

    //Keep trace of the ID order
    //this->selection_setCloud(oID);

    //Delete subsets
    cloud->remove_subset_all();

    //Delete cloud
    delete cloud;
    cloud = nullptr;

    //Delete cloud iterator in list
    list<Collection*>::iterator it = next(list_collection->begin(), oID);
    list_collection->erase(it);

    //Check for end list new selected cloud
    if(oID >= list_collection->size()){
      oID = 0;
    }

    this->update_ID_order(list_collection);
    //this->selection_setCloud(oID);
    cloud_selected = *next(list_collection->begin(), oID);

    //---------------------------
    string log = "Collection "+ name +" removed";
    console.AddLog("#", log);
  }

  //If cloud list empty
  if(list_collection->size() == 0){
    objectManager->reset_scene_object();
    cloud_selected = nullptr;
  }

  //---------------------------
}
void Scene::remove_cloud_all(){
  std::list<Collection*>* list_collection = data->get_list_collection();
  //---------------------------

  while(list_collection->size() != 0){
    Collection* cloud = *list_collection->begin();
    this->remove_cloud(cloud);
  }

  //---------------------------
}

//Reset functions
void Scene::reset_cloud(Collection* cloud){
  //---------------------------

  for(int i=0; i<cloud->list_obj.size(); i++){
    Cloud* subset = (Cloud*)cloud->get_obj(i);
    Cloud* list_obj_init = (Cloud*)cloud->get_obj_init(i);

    //Reinitialize visibility
    if(i == 0){
      subset->is_visible = true;
    }else{
      subset->is_visible = false;
    }

    //Reinitialize main data
    subset->xyz = list_obj_init->xyz;
    subset->rgb = list_obj_init->rgb;
    subset->Nxyz = list_obj_init->Nxyz;

    //Reset additional data
    subset->R.clear();
    subset->It.clear();
    subset->cosIt.clear();
    subset->root = vec3(0,0,0);

    //Transformation matrices
    subset->scale = mat4(1.0);
    subset->trans = mat4(1.0);
    subset->rotat = mat4(1.0);

    //Update
    this->update_MinMax(subset);
    this->update_buffer_location(subset);
    this->update_buffer_color(subset);

    //Reset frame
    subset->frame.reset();
  }

  //cloud->ID_obj_selected = (Cloud*)cloud->get_obj(0)->ID;

  //---------------------------
  //this->update_glyph(cloud);
}
void Scene::reset_cloud_all(){
  std::list<Collection*>* list_collection = data->get_list_collection();
  Collection* cloud_selected = data->get_cloud_selected();
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_collection->size(); i++){
    Collection* cloud = *next(list_collection->begin(),i);
    if(!cloud->is_onthefly){
      this->reset_cloud(cloud);
    }
  }

  //this->update_glyph(cloud_selected);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Update function
void Scene::update_buffer_location(Object_* object){
  //---------------------------

  if(object->obj_type == "collection"){
    Collection* cloud = (Collection*)object;
    for(int i=0; i<cloud->list_obj.size(); i++){
      Cloud* subset = (Cloud*)*next(cloud->list_obj.begin(), i);
      gpuManager->update_buffer_location(subset);
    }
  }else{
    gpuManager->update_buffer_location(object);
  }

  //---------------------------
}
void Scene::update_buffer_color(Object_* object){
  //---------------------------

  if(object->obj_type == "collection"){
    Collection* cloud = (Collection*)object;
    for(int i=0; i<cloud->list_obj.size(); i++){
      Cloud* subset = (Cloud*)*next(cloud->list_obj.begin(), i);
      gpuManager->update_buffer_color(subset);
    }
  }else{
    gpuManager->update_buffer_color(object);
  }

  //---------------------------
}
void Scene::update_glyph(Object_* object){
  if(object == nullptr) return;
  //---------------------------

  if(object->obj_type == "collection"){
    /*Collection* cloud = (Collection*)object;
    for(int i=0; i<cloud->list_obj.size(); i++){
      Cloud* subset = (Cloud*)*next(cloud->list_obj.begin(), i);
      gpuManager->update_buffer_color(subset);
    }*/
    this->update_cloud_MinMax((Collection*)object);
    objectManager->update_glyph_cloud((Collection*)object);
  }else if(object->obj_type == "subset"){
    this->update_MinMax(object);
    objectManager->update_glyph_subset((Cloud*)object);
  }

  //---------------------------
}
void Scene::update_cloud_MinMax(Collection* cloud){
  vec3 min_cloud = vec3(100, 100, 100);
  vec3 max_cloud = vec3(-100, -100, -100);
  //---------------------------

  for(int i=0; i<cloud->list_obj.size(); i++){
    Cloud* subset = (Cloud*)*next(cloud->list_obj.begin(), i);
    this->update_MinMax(subset);

    //Collection
    for(int j=0; j<3; j++){
      if ( min_cloud[j] > subset->min[j] ) min_cloud[j] = subset->min[j];
      if ( max_cloud[j] < subset->max[j] ) max_cloud[j] = subset->max[j];
    }
  }

  //---------------------------
  cloud->min = min_cloud;
  cloud->max = max_cloud;
}
void Scene::update_MinMax(Object_* object){
  vector<vec3>& XYZ = object->xyz;
  vec3 centroid = vec3(0, 0, 0);
  vec3 min = XYZ[0];
  vec3 max = XYZ[0];
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      if ( XYZ[i][j] <= min[j] ) min[j] = XYZ[i][j];
      if ( XYZ[i][j] >= max[j] ) max[j] = XYZ[i][j];
      centroid[j] += XYZ[i][j];
    }
  }

  for(int j=0;j<3;j++){
    centroid[j] /= XYZ.size();
  }

  //---------------------------
  object->min = min;
  object->max = max;
  object->COM = centroid;
}
void Scene::update_cloud_IntensityToColor(Collection* cloud){
  //---------------------------

  for(int i=0; i<cloud->list_obj.size(); i++){
    Cloud* subset = (Cloud*)*next(cloud->list_obj.begin(), i);

    vector<float>& Is = subset->I;
    vector<vec4>& RGB = subset->rgb;

    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i], 1.0f);
    }

    this->update_buffer_color(subset);
  }

  //---------------------------
}
void Scene::update_subset_IntensityToColor(Cloud* subset){
  //---------------------------

  vector<float>& Is = subset->I;
  vector<vec4>& RGB = subset->rgb;
  RGB.clear();

  for(int i=0; i<Is.size(); i++){
    vec4 new_color = vec4(Is[i], Is[i], Is[i], 1.0f);
    RGB.push_back(new_color);
  }

  this->update_buffer_color(subset);

  //---------------------------
}
void Scene::update_ID_order(list<Collection*>* list){
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Collection* cloud = *next(list->begin(),i);
    if(cloud->ID_col_order != i) cloud->ID_col_order = i;
  }

  //---------------------------
}
