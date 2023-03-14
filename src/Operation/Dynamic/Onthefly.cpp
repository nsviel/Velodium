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
void Onthefly::compute_onthefly(Collection* collection, int ID){
  //---------------------------




  if(collection->is_onthefly){
    vector<string>& list_path = collection->list_otf_path;
    list<int>& list_id = collection->list_otf_loaded;

    //Check if the cloud is already loaded
    bool already_load = false;
    for(auto i=list_id.begin(); i!=list_id.end(); i++){
      if(*i == ID){
        already_load = true;
      }
    }

    //If not, load it
    if(already_load == false && ID < list_path.size() && ID >= collection->ID_obj_otf){
      bool ok = loaderManager->load_cloud_oneFrame(collection, list_path[ID]);
      if(ok) list_id.push_back(ID);
    }

    //If too mush cloud, remove the last one
    if(collection->list_obj.size() > range){
      Object_* object = collection->get_obj(0);
      list_id.remove(object->ID);

      Object_* obj = collection->get_obj(0);
      Object_* obj_buf = collection->get_obj_buffer(0);
      Object_* obj_ini = collection->get_obj_init(0);

      collection->obj_remove_last();
    }
  }


  //---------------------------
}
void Onthefly::reset(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    if(collection->is_onthefly){
      vector<string>& list_path = collection->list_otf_path;
      list<int>& list_id = collection->list_otf_loaded;

      collection->obj_remove_all();
      collection->list_otf_loaded.clear();
      collection->ID_obj_last = 0;
      collection->ID_obj_otf = 0;
      collection->ID_obj_selected = 0;
      collection->nb_obj = 0;

      bool ok = loaderManager->load_cloud_oneFrame(collection, list_path[0]);
      if(ok) list_id.push_back(0);
    }
  }

  // Reset glyph
  Collection* cloud_selected = sceneManager->get_selected_collection();
  //sceneManager->update_glyph(collection_selected);

  //---------------------------
}
