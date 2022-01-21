#include "Player_cloud.h"

#include "Player_online.h"

#include "../../Specific/timer.h"
#include "../../Engine/Scene.h"
#include "../../Load/Saver.h"
#include "../../Load/Operation.h"

#include <experimental/filesystem>


//Constructor / Destructor
Player_cloud::Player_cloud(Player_online* online){
  //---------------------------

  this->onlineManager = online;
  this->sceneManager = new Scene();
  this->timerManager = new Timer();

  this->subset_selected = 0;
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
  string absPath = std::experimental::filesystem::current_path();
  this->saveas = absPath + "/../media/data/";

  //---------------------------
}
Player_cloud::~Player_cloud(){}

//Main function
void Player_cloud::select_byFrameID(Cloud* cloud, int frame_id){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  //---------------------------

  //If frame desired ID is superior to the number of subset restart it
  if(player_returnToZero){
    if(frame_id >= cloud->nb_subset){
      frame_id = 0;
    }
    if(frame_id < 0){
      frame_id = cloud->nb_subset - 1;
    }
  }
  else{
    if(frame_id >= cloud->nb_subset){
      frame_id = cloud->nb_subset - 1;
    }
    if(frame_id < 0){
      frame_id = 0;
    }
  }

  //Set visibility parameter for each cloud subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];
    Frame* frame = &subset->frame;

    //If selected frame
    if(i == frame_id){
      subset->visibility = true;
      cloud->subset_selected = i;

      //Check if subset has timestamp data
      if(subset->ts.size() != 0){
        frame_ID_ts = subset->ts[0];
      }

      //Make online stuff
      onlineManager->compute_onlineOpe(cloud, i);
    }
    //If several frame displayed
    else if(i >= frame_id - frame_display_range && i < frame_id && i >= 0 && frame->is_slamed){
      subset->visibility = true;
    }
    //All other frames
    else{
      subset->visibility = false;
    }
  }

  //---------------------------
  this->subset_selected = frame_id;
}
void Player_cloud::update_frame_ID(Cloud* cloud){
  //---------------------------

  //If no cloud present
  if(cloud == nullptr){
    this->subset_selected = 0;
    this->frame_max_ID = 0;
    this->frame_max_nb = 0;
    return;
  }

  //Search for frame ID
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];

    if(subset->visibility){
      this->subset_selected = i;
    }
  }

  //Set frame ID max
  this->frame_max_ID = cloud->nb_subset - 1;
  this->frame_max_nb = cloud->nb_subset;

  //---------------------------
}
void Player_cloud::supress_firstSubset(Cloud* cloud){
  vector<Subset> vec;
  //---------------------------

  for(int i=1; i<cloud->nb_subset; i++){
    cloud->subset[i].ID = i - 1;
    vec.push_back(cloud->subset[i]);
  }

  cloud->subset = vec;
  cloud->nb_subset = cloud->nb_subset - 1;

  //---------------------------
}

//Player functions
void Player_cloud::player_runtime(){
  //Continually running, wait for flag to proceed
  //---------------------------

  if(player_flag_1s){
    subset_selected++;
    Cloud* cloud = sceneManager->get_cloud_selected();
    this->select_byFrameID(cloud, subset_selected);

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
        subset_selected = 0;
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
  subset_selected = 0;
  this->select_byFrameID(cloud, subset_selected);

  //---------------------------
}
void Player_cloud::player_save(Cloud* cloud){
  Saver saverManager;
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];
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
