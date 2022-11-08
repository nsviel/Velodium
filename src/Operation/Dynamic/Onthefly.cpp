#include "Onthefly.h"

#include "../Node_operation.h"

#include "../../Specific/fct_system.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"
#include "../../Load/Processing/Loader.h"

#include "../../Operation/Transformation/Transformation.h"
#include "../../Specific/fct_maths.h"


//Constructor / Destructor
Onthefly::Onthefly(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();

  //---------------------------
  this->update_configuration();
}
Onthefly::~Onthefly(){}

void Onthefly::update_configuration(){
  //---------------------------

  this->range = 30;

  //---------------------------
}
void Onthefly::compute_onthefly(Cloud* cloud, int ID){
  //---------------------------

  if(cloud->onthefly){
    vector<string>& list_path = cloud->list_path;
    list<int>& list_id = cloud->list_loaded;

    //Check if the subset is already loaded
    bool already_load = false;
    for(auto i=list_id.begin(); i!=list_id.end(); i++){
      if(*i == ID){
        already_load = true;
      }
    }

    //If not, load it
    if(already_load == false && ID < list_path.size() && ID >= cloud->ID_file){
      bool ok = loaderManager->load_cloud_oneFrame(cloud, list_path[ID]);
      if(ok) list_id.push_back(ID);
    }

    //If too mush subset, remove the last one
    if(cloud->subset.size() > range){
      Subset* subset = *next(cloud->subset.begin(), 0);
      list_id.remove(subset->ID);
      sceneManager->remove_subset_last(cloud);
    }
  }

  //---------------------------
}
