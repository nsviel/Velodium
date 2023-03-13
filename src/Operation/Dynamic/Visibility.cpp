#include "Visibility.h"
#include "Onthefly.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"


//Constructor / Destructor
Visibility::Visibility(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->flyManager = node_ope->get_flyManager();

  this->visibility_range = 15;
  this->visibility_range_max = 30;

  //---------------------------
}
Visibility::~Visibility(){}

//Main function
void Visibility::compute_visibility(Collection* collection, int& ID_object){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(ID_object);
  if(cloud == nullptr) return;
  //---------------------------

  //Set visibility just for wanted subsets
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);

    if(cloud->ID >= ID_object - visibility_range + 1 && cloud->ID <= ID_object){
      cloud->is_visible = true;
    }else{
      cloud->is_visible = false;
    }
  }

  //---------------------------
}
void Visibility::compute_visibility_range_max(Collection* collection){
  if(collection == nullptr) return;
  //---------------------------

  if(collection->nb_obj < visibility_range_max && visibility_range_max > 15){
    this->visibility_range_max = collection->nb_obj;
  }

  //---------------------------
}
