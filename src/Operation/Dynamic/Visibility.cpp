#include "Visibility.h"
#include "Onthefly.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"


//Constructor / Destructor
Visibility::Visibility(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  this->sceneManager = node_engine->get_sceneManager();
  this->flyManager = node_ope->get_flyManager();

  this->visibility_range = 15;
  this->visibility_range_max = 30;

  //---------------------------
}
Visibility::~Visibility(){}

//Main function
void Visibility::compute_visibility(Cloud* cloud, int& ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  //Set visibility just for wanted subsets
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID >= ID_subset - visibility_range + 1 && subset->ID <= ID_subset){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }
  }

  //---------------------------
}
void Visibility::compute_visibility_range_max(Cloud* cloud){
  if(cloud == nullptr) return;
  //---------------------------

  if(cloud->nb_subset < visibility_range_max && visibility_range_max > 15){
    this->visibility_range_max = cloud->nb_subset;
  }

  //---------------------------
}
