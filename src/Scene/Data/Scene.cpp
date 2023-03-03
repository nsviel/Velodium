#include "Scene.h"

#include "../Node_scene.h"
#include "../Glyph/Object.h"
#include "../../Engine/OpenGL/GPU_transfert.h"


//Constructor / Destructor
Scene::Scene(Node_scene* node){
  //---------------------------

  this->objectManager = node->get_objectManager();
  this->gpuManager = new GPU_transfert();

  this->list_cloud = new list<Cloud*>;
  this->cloud_selected = nullptr;
  this->ID_cloud = 0;

  //---------------------------
}
Scene::~Scene(){
  //---------------------------

  delete list_cloud;

  //---------------------------
}

//Remove functions
void Scene::remove_cloud(Cloud* cloud){
  //---------------------------

  if(!get_is_list_empty()){
    int oID = cloud->ID_order;
    string name =  cloud->name;
    //---------------------------

    //Keep trace of the ID order
    this->selection_setCloud(oID);

    //Delete subsets
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      this->remove_subset(cloud, subset->ID);
    }

    //Delete cloud
    delete cloud;
    cloud = nullptr;

    //Delete cloud iterator in list
    list<Cloud*>::iterator it = next(list_cloud->begin(), oID);
    list_cloud->erase(it);

    //Check for end list new selected cloud
    if(oID >= list_cloud->size()){
      oID = 0;
    }

    this->update_cloud_oID(list_cloud);
    this->selection_setCloud(oID);

    //---------------------------
    string log = "Cloud "+ name +" removed";
    console.AddLog("#", log);
  }

  //If cloud list empty
  if(list_cloud->size() == 0){
    objectManager->reset_scene_object();
    this->cloud_selected = nullptr;
  }

  //---------------------------
}
void Scene::remove_cloud_all(){
  //---------------------------

  while(list_cloud->size() != 0){
    Cloud* cloud = *list_cloud->begin();
    this->remove_cloud(cloud);
  }

  //---------------------------
}
void Scene::remove_subset(Cloud* cloud, int ID){
  //---------------------------

  //Can just remove last or first subset
  Subset* subset_first = get_subset(cloud, 0);
  Subset* subset_last = get_subset(cloud, cloud->nb_subset-1);
  int oID;

  if(ID == subset_first->ID){
    oID = 0;
  }else if(ID == subset_last->ID){
    oID = cloud->nb_subset-1;
  }else{
    return;
  }

  //Get subset object
  Subset* subset = get_subset(cloud, 0);
  Subset* subset_buf = get_subset_buffer(cloud, 0);
  Subset* subset_ini = get_subset_init(cloud, 0);

  //Remove data from GPU
  gpuManager->unbind_object(subset);

  //Supress Subset pointer
  cloud->subset.pop_front();
  cloud->subset_buffer.pop_front();
  cloud->subset_init.pop_front();

  //Delete Subset object
  delete subset;
  delete subset_buf;
  delete subset_ini;

  //---------------------------
  cloud->nb_subset = cloud->subset.size();
}
void Scene::remove_subset_last(Cloud* cloud){
  //---------------------------

  //Get subset object
  Subset* subset = get_subset(cloud, 0);
  Subset* subset_buf = get_subset_buffer(cloud, 0);
  Subset* subset_ini = get_subset_init(cloud, 0);

  //Remove data from GPU
  gpuManager->unbind_object(subset);

  //Supress Subset pointer
  cloud->subset.pop_front();
  cloud->subset_buffer.pop_front();
  cloud->subset_init.pop_front();

  //Delete Subset object
  delete subset;
  delete subset_buf;
  delete subset_ini;

  //---------------------------
  cloud->nb_subset = cloud->subset.size();
}
void Scene::remove_subset_all(Cloud* cloud){
  int size = cloud->subset.size();
  //---------------------------

  for(int i=0; i<size; i++){
    this->remove_subset_last(cloud);
  }

  //---------------------------
}

//Adding functions
void Scene::add_new_subset(Cloud* cloud, Subset* subset){
  //---------------------------

  cloud->add_new_subset(subset);
  objectManager->update_glyph_subset(subset);

  //---------------------------
}

//Reset functions
void Scene::reset_cloud(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    Subset* subset_init = get_subset_init(cloud, i);

    //Reinitialize visibility
    if(i == 0){
      subset->is_visible = true;
    }else{
      subset->is_visible = false;
    }

    //Reinitialize main data
    subset->xyz = subset_init->xyz;
    subset->rgb = subset_init->rgb;
    subset->Nxyz = subset_init->Nxyz;

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
    this->update_subset_MinMax(subset);
    this->update_buffer_location(subset);
    this->update_buffer_color(subset);

    //Reset frame
    subset->frame.reset();
  }

  cloud->ID_selected = get_subset(cloud, 0)->ID;

  //---------------------------
  this->update_glyph(cloud);
}
void Scene::reset_cloud_all(){
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    if(!cloud->is_onthefly){
      this->reset_cloud(cloud);
    }
  }

  this->update_glyph(cloud_selected);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Update function
void Scene::update_buffer_location(Object_* object){
  //---------------------------

  if(object->obj_type == "cloud"){
    Cloud* cloud = (Cloud*)object;
    for(int i=0; i<cloud->subset.size(); i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      gpuManager->update_buffer_location(subset);
    }
  }else if(object->obj_type == "subset"){
    gpuManager->update_buffer_location(object);
  }

  //---------------------------
}
void Scene::update_buffer_color(Object_* object){
  //---------------------------

  if(object->obj_type == "cloud"){
    Cloud* cloud = (Cloud*)object;
    for(int i=0; i<cloud->subset.size(); i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      gpuManager->update_buffer_color(subset);
    }
  }else if(object->obj_type == "subset"){
    gpuManager->update_buffer_color(object);
  }

  //---------------------------
}
void Scene::update_glyph(Object_* object){
  if(object == nullptr) return;
  //---------------------------

  if(object->obj_type == "cloud"){
    /*Cloud* cloud = (Cloud*)object;
    for(int i=0; i<cloud->subset.size(); i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      gpuManager->update_buffer_color(subset);
    }*/
    this->update_cloud_MinMax((Cloud*)object);
    objectManager->update_glyph_cloud((Cloud*)object);
  }else if(object->obj_type == "subset"){
    this->update_subset_MinMax((Subset*)object);
    objectManager->update_glyph_subset((Subset*)object);
  }

  //---------------------------
}
void Scene::update_cloud_MinMax(Cloud* cloud){
  vec3 min_cloud = vec3(100, 100, 100);
  vec3 max_cloud = vec3(-100, -100, -100);
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->update_subset_MinMax(subset);

    //Cloud
    for(int j=0; j<3; j++){
      if ( min_cloud[j] > subset->min[j] ) min_cloud[j] = subset->min[j];
      if ( max_cloud[j] < subset->max[j] ) max_cloud[j] = subset->max[j];
    }
  }

  //---------------------------
  cloud->min = min_cloud;
  cloud->max = max_cloud;
}
void Scene::update_subset_MinMax(Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
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
  subset->min = min;
  subset->max = max;
  subset->COM = centroid;
}
void Scene::update_cloud_IntensityToColor(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    vector<float>& Is = subset->I;
    vector<vec4>& RGB = subset->rgb;

    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i], 1.0f);
    }

    this->update_buffer_color(subset);
  }

  //---------------------------
}
void Scene::update_cloud_oID(list<Cloud*>* list){
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);
    if(cloud->ID_order != i) cloud->ID_order = i;
  }

  //---------------------------
}
void Scene::update_subset_IntensityToColor(Subset* subset){
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
