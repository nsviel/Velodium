#include "Player.h"
#include "Online.h"
#include "Onthefly.h"

#include "../Node_operation.h"

#include "../../Specific/Function/fct_timer.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Zenity.h"
#include "../../Specific/File/Path.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Engine/Core/Configuration.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"
#include "../../Load/Processing/Loader.h"


//Constructor / Destructor
Player::Player(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->objectManager = node_scene->get_objectManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->saverManager = node_load->get_saverManager();
  this->loaderManager = node_load->get_loaderManager();
  this->flyManager = node_ope->get_flyManager();
  this->timerManager = new Timer();

  //---------------------------
  this->update_configuration();
}
Player::~Player(){}

void Player::update_configuration(){
  //---------------------------

  this->player_frequency = 10;
  this->player_isrunning = false;
  this->player_ispaused = false;
  this->player_returnToZero = false;
  this->player_time_flag = false;
  this->player_saveas = get_path_abs_build() + "../media/data/";
  this->player_mode = configManager->parse_json_s("dynamic", "player_mode");

  //---------------------------
}
void Player::runtime(){
  //Continually running, wait for flag to proceed
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  if(player_time_flag && collection != nullptr){
    this->select_byObjectID(collection, collection->ID_obj_selected + 1);

    player_time_flag = false;
  }

  //---------------------------
}

//Selection functions
void Player::select_byObjectID(Collection* collection, int ID_object){
  if(collection == nullptr) return;
  //---------------------------

  //If on the fly option, load cloud
  flyManager->compute_onthefly(collection, ID_object);

  //If in side range, make operation on cloud
  if(compute_range_limit(collection, ID_object)){
    onlineManager->compute_onlineOpe(collection, ID_object);
  }

  //Update glyphs
  Object_* object = collection->get_obj_byID(ID_object);
  //objectManager->update_glyph_cloud(object);

  //---------------------------
  collection->ID_obj_selected = ID_object;
  say(collection->ID_obj_selected);
}
void Player::compute_wheel_selection(string direction){
  Collection* collection = sceneManager->get_selected_collection();
  //----------------------------

  //Wheel - rolling stone
  if(collection != nullptr){
    int ID_obj = collection->ID_obj_selected;

    if(direction == "up"){
      ID_obj++;
    }
    else if(direction =="down"){
      ID_obj--;
    }

    this->select_byObjectID(collection, ID_obj);
  }

  //----------------------------
}
bool Player::compute_range_limit(Collection* collection, int& ID_object){
  Cloud* subset_first = (Cloud*)collection->get_obj(0);
  Cloud* subset_last = (Cloud*)collection->get_obj(collection->nb_obj-1);
  //---------------------------
//PROBLEM DE ID ICI JE PENSE
  //Check if cloud exists
  Cloud* cloud = (Cloud*)collection->get_obj(ID_object);
  if(cloud == nullptr){
    return false;
  }

  //If frame desired ID is superior to the number of cloud restart it
  if(player_returnToZero){
    if(ID_object > subset_last->ID){
      ID_object = subset_first->ID;
    }
    if(ID_object < subset_first->ID){
      ID_object = subset_last->ID;
    }
  }
  else{
    if(ID_object > subset_last->ID){
      ID_object = subset_last->ID;
      return false;
    }
    if(ID_object < subset_first->ID){
      ID_object = subset_first->ID;
      return false;
    }
  }

  //Set visibility parameter for each collection cloud
  collection->ID_obj_selected = ID_object;

  //---------------------------
  return true;
}

//Player functions
void Player::player_start(){
  this->player_isrunning = true;
  this->player_ispaused = false;
  //---------------------------

  if(timerManager->isRunning() == false){
    //Set timer parameter
    float increment = (1 / (float)player_frequency) * 1000;
    timerManager->setFunc([&](){
      if(!sceneManager->get_is_list_empty()){
        player_time_flag = true;
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
void Player::player_pause(){
  this->player_isrunning = false;
  this->player_ispaused = true;
  //---------------------------

  timerManager->stop();

  //---------------------------
}
void Player::player_start_or_pause(){
  //---------------------------

  if(player_ispaused){
    this->player_start();
  }else{
    this->player_pause();
  }

  //---------------------------
}
void Player::player_stop(){
  Collection* collection = sceneManager->get_selected_collection();
  this->player_isrunning = false;
  this->player_ispaused = false;
  //---------------------------

  timerManager->stop();
  this->select_byObjectID(collection, 0);

  //---------------------------
}
void Player::player_save(Collection* collection){
  //---------------------------

  //Save each cloud
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    saverManager->save_subset(cloud, "ply", player_saveas);
  }

  //---------------------------
}
void Player::player_selectDirSave(){
  //---------------------------

  zenity_directory(player_saveas);

  //---------------------------
}
void Player::player_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->player_frequency = value;

  //---------------------------
}
