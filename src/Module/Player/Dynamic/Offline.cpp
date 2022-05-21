#include "Offline.h"

#include "Online.h"

#include "../Player_node.h"

#include "../../Module_node.h"

#include "../../../Specific/fct_timer.h"
#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Object.h"
#include "../../../Engine/Scene/Configuration.h"

#include "../../../Load/Load_node.h"
#include "../../../Load/Processing/Saver.h"


//Constructor / Destructor
Offline::Offline(Player_node* node){
  //---------------------------

  Module_node* node_module = node->get_node_module();
  Engine_node* node_engine = node_module->get_node_engine();
  Load_node* node_load = node_engine->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->onlineManager = node->get_onlineManager();
  this->objectManager = node_engine->get_objectManager();;
  this->sceneManager = node_engine->get_sceneManager();
  this->saveManager = node_load->get_saveManager();
  this->timerManager = new Timer();

  //---------------------------
  this->update_configuration();
}
Offline::~Offline(){}

void Offline::update_configuration(){
  //---------------------------

  this->player_frequency = 10;
  this->player_isrunning = false;
  this->player_ispaused = false;
  this->player_returnToZero = false;
  this->player_flag_1s = false;
  this->player_saveas = get_absolutePath_build() + "../media/data/";
  this->player_mode = configManager->parse_json_s("module", "player_mode");

  //---------------------------
}

//Selection functions
void Offline::select_bySubsetID(Cloud* cloud, int ID_subset){
  //---------------------------

  //If in side range, make operation on subset
  bool range_ok = select_rangeLimit(cloud, ID_subset);
  if(range_ok){
    onlineManager->compute_onlineOpe(cloud, ID_subset);
  }

  //Update glyphs
  objectManager->update_glyph_cloud(cloud);

  //---------------------------
}
bool Offline::select_rangeLimit(Cloud* cloud, int& ID_subset){
  Subset* subset_first = sceneManager->get_subset(cloud, 0);
  Subset* subset_last = sceneManager->get_subset(cloud, cloud->nb_subset-1);
  //---------------------------

  //Check if subset exists
  Subset* subset = sceneManager->get_subset(cloud, ID_subset);
  if(subset == nullptr){
    return false;
  }

  //If frame desired ID is superior to the number of subset restart it
  if(player_returnToZero){
    if(ID_subset > subset_last->ID){
      ID_subset = subset_first->ID;
    }
    if(ID_subset < subset_first->ID){
      ID_subset = subset_last->ID;
    }
  }
  else{
    if(ID_subset > subset_last->ID){
      ID_subset = subset_last->ID;
      return false;
    }
    if(ID_subset < subset_first->ID){
      ID_subset = subset_first->ID;
      return false;
    }
  }

  //Set visibility parameter for each cloud subset
  cloud->ID_selected = ID_subset;

  //---------------------------
  return true;
}

//Player functions
void Offline::player_runtime(){
  //Continually running, wait for flag to proceed
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(player_flag_1s && cloud != nullptr){
    this->select_bySubsetID(cloud, cloud->ID_selected + 1);

    player_flag_1s = false;
  }

  //---------------------------
}
void Offline::player_start(){
  this->player_isrunning = true;
  this->player_ispaused = false;
  //---------------------------

  if(timerManager->isRunning() == false){
    //Set timer parameter
    float increment = (1 / (float)player_frequency) * 1000;
    timerManager->setFunc([&](){
      if(!sceneManager->get_is_list_empty()){
        player_flag_1s = true;
      }else{
        timerManager->stop();
      }
    });
    timerManager->setInterval(increment);

    //Start timer
    timerManager->start();
  }

  //---------------------------
}
void Offline::player_pause(){
  this->player_isrunning = false;
  this->player_ispaused = true;
  //---------------------------

  timerManager->stop();

  //---------------------------
}
void Offline::player_start_or_pause(){
  //---------------------------

  if(player_ispaused){
    this->player_start();
  }else{
    this->player_pause();
  }

  //---------------------------
}
void Offline::player_stop(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  this->player_isrunning = false;
  this->player_ispaused = false;
  //---------------------------

  timerManager->stop();
  this->select_bySubsetID(cloud, 0);

  //---------------------------
}
void Offline::player_save(Cloud* cloud){
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    saveManager->save_subset(subset, "ply", player_saveas);
  }

  //---------------------------
}
void Offline::player_selectDirSave(){
  //---------------------------

  string path;
  zenity_directory("", path);

  this->player_saveas = path + "/";

  //---------------------------
}
void Offline::player_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->player_frequency = value;

  //---------------------------
}
