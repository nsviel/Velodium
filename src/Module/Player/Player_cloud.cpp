#include "Player_cloud.h"

#include "Player_online.h"

#include "../../Specific/fct_timer.h"
#include "../../Specific/fct_system.h"
#include "../../Engine/Scene.h"
#include "../../Load/Saver.h"
#include "../../Load/Operation.h"



//Constructor / Destructor
Player_cloud::Player_cloud(Player_online* online){
  //---------------------------

  this->onlineManager = online;
  this->sceneManager = new Scene();
  this->timerManager = new Timer();

  this->frame_max_ID = 0;
  this->frame_max_nb = 0;
  this->frame_ID_ts = 0;
  this->frame_display_range = 15;

  this->player_frequency = 10;
  this->player_isrunning = false;
  this->player_ispaused = false;
  this->player_returnToZero = false;
  this->player_flag_1s = false;

  //Get absolute executable location
  this->saveas = get_absolutePath_build() + "/../media/data/";

  //---------------------------
}
Player_cloud::~Player_cloud(){}

//Selection functions
void Player_cloud::select_byFrameID(Cloud* cloud, int frame_ID){
  //---------------------------

  this->select_rangeLimit(cloud, frame_ID);
  this->select_setVisibility(cloud, frame_ID);
  onlineManager->compute_onlineOpe(cloud, frame_ID);

  //---------------------------
}
void Player_cloud::select_rangeLimit(Cloud* cloud, int& frame_ID){
  //---------------------------

  //If frame desired ID is superior to the number of subset restart it
  if(player_returnToZero){
    if(frame_ID >= cloud->nb_subset){
      frame_ID = 0;
    }
    if(frame_ID < 0){
      frame_ID = cloud->nb_subset - 1;
    }
  }
  else{
    if(frame_ID >= cloud->nb_subset){
      frame_ID = cloud->nb_subset - 1;
      return;
    }
    if(frame_ID < 0){
      frame_ID = 0;
      return;
    }
  }

  //---------------------------
}
void Player_cloud::select_setVisibility(Cloud* cloud, int& frame_ID){
  Subset* subset = sceneManager->get_subset(cloud, frame_ID);
  //---------------------------

  //Set visibility parameter for each cloud subset
  cloud->subset_selected = frame_ID;

  //Check if subset has timestamp data
  if(subset->ts.size() != 0){
    this->frame_ID_ts = subset->ts[0];
  }

  //Unset alll other subset visibility
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    subset->visibility = false;
  }

  //Set visibility just for wanted susbsets
  for(int i=0; i<frame_display_range; i++){
    int trained_frame = frame_ID - frame_display_range + i + 1;

    if(trained_frame >= 0){
      Subset* subset = sceneManager->get_subset(cloud, trained_frame);
      subset->visibility = true;
    }
  }

  //---------------------------
}
void Player_cloud::update_player_params(Cloud* cloud){
  //---------------------------

  //If no cloud present
  if(cloud == nullptr){
    this->frame_max_ID = 0;
    this->frame_max_nb = 0;
  }else{
    this->frame_max_ID = cloud->nb_subset - 1;
    this->frame_max_nb = cloud->nb_subset;
  }

  //---------------------------
}

//Player functions
void Player_cloud::player_runtime(){
  //Continually running, wait for flag to proceed
  //---------------------------

  if(player_flag_1s){
    int subset_selected_ID = sceneManager->get_subset_selected_ID();
    Cloud* cloud = sceneManager->get_cloud_selected();
    this->select_byFrameID(cloud, subset_selected_ID + 1);

    player_flag_1s = false;
  }

  //---------------------------
}
void Player_cloud::player_start(){
  this->player_isrunning = true;
  this->player_ispaused = false;
  //---------------------------

  if(timerManager->isRunning() == false){
    //Set timer parameter
    float increment = (1 / (float)player_frequency) * 1000;
    timerManager->setFunc([&](){
      if(sceneManager->is_atLeastOnecloud()){
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
void Player_cloud::player_pause(){
  this->player_isrunning = false;
  this->player_ispaused = true;
  //---------------------------

  timerManager->stop();

  //---------------------------
}
void Player_cloud::player_stop(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  this->player_isrunning = false;
  this->player_ispaused = false;
  //---------------------------

  timerManager->stop();
  this->select_byFrameID(cloud, 0);

  //---------------------------
}
void Player_cloud::player_save(Cloud* cloud){
  Saver saverManager;
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    saverManager.save_subset(subset, "ply", saveas);
  }

  //---------------------------
}
void Player_cloud::player_selectDirSave(){
  //---------------------------

  string path;
  Operation opeManager;
  opeManager.selectDirectory(path);

  this->saveas = path + "/";

  //---------------------------
}
void Player_cloud::player_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->player_frequency = value;

  //---------------------------
}
