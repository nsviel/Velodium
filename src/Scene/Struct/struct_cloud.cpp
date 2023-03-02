#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  this->ID = 0;
  this->oID = 0;

  this->ID_selected = 0;
  this->ID_subset = 0;
  this->ID_file = 0;

  //Parameters
  this->nb_subset = 0;
  this->nb_point = 0;
  this->point_size = 1;

  //Flags
  this->visibility = true;
  this->heatmap = false;
  this->onthefly = false;
  this->boxed = false;

  //Infos
  this->path = "";
  this->name = "";
  this->format = "";
  this->saveas = "";
  this->draw_type = "";
  this->dataFormat = "";
  this->lidar_model = "";

  //Transformation
  this->min = glm::vec3(0.0f);
  this->max = glm::vec3(0.0f);
  this->root = glm::vec3(0.0f);
  this->COM = glm::vec3(0.0f);
  this->unicolor = glm::vec4(1.0f);

  //Data
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
