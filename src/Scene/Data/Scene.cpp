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
void Scene::remove_collection(Collection* collection){
  std::list<Collection*>* list_collection = data->get_list_collection();
  Collection* cloud_selected = data->get_cloud_selected();
  //---------------------------

  if(!get_is_list_empty()){
    int oID = collection->ID_col_order;
    string name =  collection->name;
    //---------------------------

    //Keep trace of the ID order
    //this->selection_setCloud(oID);

    //Delete subsets
    collection->obj_remove_all();

    //Delete collection
    delete collection;
    collection = nullptr;

    //Delete collection iterator in list
    list<Collection*>::iterator it = next(list_collection->begin(), oID);
    list_collection->erase(it);

    //Check for end list new selected collection
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

  //If collection list empty
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
    Collection* collection = *list_collection->begin();
    this->remove_collection(collection);
  }

  //---------------------------
}

//Reset functions
void Scene::reset_collection(Collection* collection){
  //---------------------------

  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    Cloud* list_obj_init = (Cloud*)collection->get_obj_init(i);

    //Reinitialize visibility
    if(i == 0){
      cloud->is_visible = true;
    }else{
      cloud->is_visible = false;
    }

    //Reinitialize main data
    cloud->xyz = list_obj_init->xyz;
    cloud->rgb = list_obj_init->rgb;
    cloud->Nxyz = list_obj_init->Nxyz;

    //Reset additional data
    cloud->R.clear();
    cloud->It.clear();
    cloud->cosIt.clear();
    cloud->root = vec3(0,0,0);

    //Transformation matrices
    cloud->scale = mat4(1.0);
    cloud->trans = mat4(1.0);
    cloud->rotat = mat4(1.0);

    //Update
    this->update_MinMax(cloud);
    this->update_buffer_location(cloud);
    this->update_buffer_color(cloud);

    //Reset frame
    cloud->frame.reset();
  }

  //collection->ID_obj_selected = (Cloud*)collection->get_obj(0)->ID;

  //---------------------------
  //this->update_glyph(collection);
}
void Scene::reset_collection_all(){
  std::list<Collection*>* list_collection = data->get_list_collection();
  Collection* cloud_selected = data->get_cloud_selected();
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    if(!collection->is_onthefly){
      this->reset_collection(collection);
    }
  }

  //this->update_glyph(collection_selected);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Update function
void Scene::update_buffer_location(Object_* object){
  //---------------------------

  /*
    Collection* collection = (Collection*)object;
    for(int i=0; i<collection->list_obj.size(); i++){
      Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
      gpuManager->update_buffer_location(cloud);
  */

  gpuManager->update_buffer_location(object);

  //---------------------------
}
void Scene::update_buffer_color(Object_* object){
  //---------------------------
  /*
    Collection* collection = (Collection*)object;
    for(int i=0; i<collection->list_obj.size(); i++){
      Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
      gpuManager->update_buffer_color(cloud);
    */


    gpuManager->update_buffer_color(object);


  //---------------------------
}
void Scene::update_glyph(Object_* object){
  if(object == nullptr) return;
  //---------------------------


    /*Collection* collection = (Collection*)object;
    for(int i=0; i<collection->list_obj.size(); i++){
      Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
      gpuManager->update_buffer_color(cloud);
    }*/


  this->update_MinMax(object);
  objectManager->update_glyph_subset((Cloud*)object);

  //---------------------------
}
void Scene::update_col_MinMax(Collection* collection){
  vec3 min_cloud = vec3(100, 100, 100);
  vec3 max_cloud = vec3(-100, -100, -100);
  //---------------------------

  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    this->update_MinMax(cloud);

    //Collection
    for(int j=0; j<3; j++){
      if ( min_cloud[j] > cloud->min[j] ) min_cloud[j] = cloud->min[j];
      if ( max_cloud[j] < cloud->max[j] ) max_cloud[j] = cloud->max[j];
    }
  }

  //---------------------------
  collection->min = min_cloud;
  collection->max = max_cloud;
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
void Scene::update_cloud_IntensityToColor(Collection* collection){
  //---------------------------

  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);

    vector<float>& Is = cloud->I;
    vector<vec4>& RGB = cloud->rgb;

    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i], 1.0f);
    }

    this->update_buffer_color(cloud);
  }

  //---------------------------
}
void Scene::update_subset_IntensityToColor(Cloud* cloud){
  //---------------------------

  vector<float>& Is = cloud->I;
  vector<vec4>& RGB = cloud->rgb;
  RGB.clear();

  for(int i=0; i<Is.size(); i++){
    vec4 new_color = vec4(Is[i], Is[i], Is[i], 1.0f);
    RGB.push_back(new_color);
  }

  this->update_buffer_color(cloud);

  //---------------------------
}
void Scene::update_ID_order(list<Collection*>* list){
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Collection* collection = *next(list->begin(),i);
    if(collection->ID_col_order != i) collection->ID_col_order = i;
  }

  //---------------------------
}
