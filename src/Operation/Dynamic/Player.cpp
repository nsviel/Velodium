#include "Player.h"
#include "Online.h"

#include "../Node_operation.h"

#include "../../Specific/fct_timer.h"
#include "../../Specific/fct_system.h"
#include "../../Specific/fct_zenity.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"
#include "../../Load/Processing/Loader.h"


//Constructor / Destructor
Player::Player(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->objectManager = node_engine->get_objectManager();;
  this->sceneManager = node_engine->get_sceneManager();
  this->saveManager = node_load->get_saveManager();
  this->loadManager = node_load->get_loadManager();
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
  this->player_saveas = get_absolutePath_build() + "../media/data/";
  this->player_mode = configManager->parse_json_s("module", "player_mode");

  //---------------------------
}
void Player::runtime(){
  //Continually running, wait for flag to proceed
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(player_time_flag && cloud != nullptr){
    this->select_bySubsetID(cloud, cloud->ID_selected + 1);

    player_time_flag = false;
  }

  //---------------------------
}

//Selection functions
void Player::select_bySubsetID(Cloud* cloud, int ID_subset){
  if(cloud == nullptr) return;
  //---------------------------

  //If on the fly option, load subset
  if(cloud->onthefly){
    this->compute_onthefly(cloud);
  }

  //If in side range, make operation on subset
  if(compute_range_limit(cloud, ID_subset)){
    onlineManager->compute_onlineOpe(cloud, ID_subset);
  }

  //Update glyphs
  Subset* subset = sceneManager->get_subset(cloud, ID_subset);
  objectManager->update_glyph_subset(subset);

  //---------------------------
}
void Player::compute_onthefly(Cloud* cloud){
  int range = onlineManager->get_visibility_range();
  //---------------------------

  loadManager->load_cloud_oneFrame(cloud);
  if(cloud->subset.size() > range){
    sceneManager->remove_subset_last(cloud);
  }

  //---------------------------
}
void Player::compute_wheel_selection(string direction){
  Cloud* cloud = sceneManager->get_selected_cloud();
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
bool Player::compute_range_limit(Cloud* cloud, int& ID_subset){
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
  Cloud* cloud = sceneManager->get_selected_cloud();
  this->player_isrunning = false;
  this->player_ispaused = false;
  //---------------------------

  timerManager->stop();
  this->select_bySubsetID(cloud, 0);

  //---------------------------
}
void Player::player_save(Cloud* cloud){
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    saveManager->save_subset(subset, "ply", player_saveas);
  }

  //---------------------------
}
void Player::player_selectDirSave(){
  //---------------------------

  string path;
  zenity_directory("", path);

  this->player_saveas = path + "/";

  //---------------------------
}
void Player::player_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->player_frequency = value;

  //---------------------------
}
