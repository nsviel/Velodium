#include "Onthefly.h"

#include "../Node_operation.h"

#include "../../Specific/File/Directory.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"
#include "../../Load/Processing/Loader.h"

#include "../../Operation/Transformation/Transformation.h"
#include "../../Specific/Function/fct_math.h"


//Constructor / Destructor
Onthefly::Onthefly(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_scene->get_sceneManager();
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
void Onthefly::compute_onthefly(Collection* cloud, int ID){
  //---------------------------

  if(cloud->is_onthefly){
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
    if(already_load == false && ID < list_path.size() && ID >= cloud->ID_onthefly){
      bool ok = loaderManager->load_cloud_oneFrame(cloud, list_path[ID]);
      if(ok) list_id.push_back(ID);
    }

    //If too mush subset, remove the last one
    if(cloud->subset.size() > range){
      Cloud* subset = cloud->get_subset(0);
      list_id.remove(subset->ID);
      cloud->remove_subset_last();
    }
  }

  //---------------------------
}
void Onthefly::reset(){
  list<Collection*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_cloud->size(); i++){
    Collection* cloud = *next(list_cloud->begin(),i);
    if(cloud->is_onthefly){
      vector<string>& list_path = cloud->list_path;
      list<int>& list_id = cloud->list_loaded;

      cloud->remove_subset_all();
      cloud->list_loaded.clear();
      cloud->ID_subset = 0;
      cloud->ID_onthefly = 0;
      cloud->ID_selected = 0;
      cloud->nb_subset = 0;

      bool ok = loaderManager->load_cloud_oneFrame(cloud, list_path[0]);
      if(ok) list_id.push_back(0);
    }
  }

  // Reset glyph
  Collection* cloud_selected = sceneManager->get_selected_cloud();
  sceneManager->update_glyph(cloud_selected);

  //---------------------------
}
