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
  Collection* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(player_time_flag && cloud != nullptr){
    this->select_bySubsetID(cloud, cloud->ID_selected + 1);

    player_time_flag = false;
  }

  //---------------------------
}

//Selection functions
void Player::select_bySubsetID(Collection* cloud, int ID_subset){
  if(cloud == nullptr) return;
  //---------------------------

  //If on the fly option, load subset
  flyManager->compute_onthefly(cloud, ID_subset);

  //If in side range, make operation on subset
  if(compute_range_limit(cloud, ID_subset)){
    onlineManager->compute_onlineOpe(cloud, ID_subset);
  }

  //Update glyphs
  Subset* subset = cloud->get_subset_byID(ID_subset);
  objectManager->update_glyph_subset(subset);

  //---------------------------
}
void Player::compute_wheel_selection(string direction){
  Collection* cloud = sceneManager->get_selected_cloud();
  //----------------------------

  //Wheel - rolling stone
  if(cloud != nullptr){
    int subset_selected_ID = cloud->ID_selected;

    if(direction == "up"){
      subset_selected_ID++;
    }
    else if(direction =="down"){
      subset_selected_ID--;
    }

    this->select_bySubsetID(cloud, subset_selected_ID);
  }

  //----------------------------
}
bool Player::compute_range_limit(Collection* cloud, int& ID_subset){
  Subset* subset_first = cloud->get_subset(0);
  Subset* subset_last = cloud->get_subset(cloud->nb_subset-1);
  //---------------------------
//PROBLEM DE ID ICI JE PENSE
  //Check if subset exists
  Subset* subset = cloud->get_subset(ID_subset);
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
  Collection* cloud = sceneManager->get_selected_cloud();
  this->player_isrunning = false;
  this->player_ispaused = false;
  //---------------------------

  timerManager->stop();
  this->select_bySubsetID(cloud, 0);

  //---------------------------
}
void Player::player_save(Collection* cloud){
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = cloud->get_subset(i);
    saverManager->save_subset(subset, "ply", player_saveas);
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
