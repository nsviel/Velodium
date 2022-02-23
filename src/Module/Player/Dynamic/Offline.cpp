#include "Offline.h"

#include "Online.h"

#include "../../../Specific/fct_timer.h"
#include "../../../Specific/fct_system.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Load/Saver.h"
#include "../../../Load/Pather.h"


//Constructor / Destructor
Offline::Offline(Online* online){
  //---------------------------

  this->onlineManager = online;
  this->sceneManager = new Scene();
  this->timerManager = new Timer();

  this->player_frequency = 10;
  this->player_isrunning = false;
  this->player_ispaused = false;
  this->player_returnToZero = false;
  this->player_flag_1s = false;
  this->player_saveas = get_absolutePath_build() + "/../media/data/";

  //---------------------------
}
Offline::~Offline(){}

//Selection functions
void Offline::select_bySubsetID(Cloud* cloud, int ID_subset){
  //---------------------------

  bool range_ok = select_rangeLimit(cloud, ID_subset);
  if(range_ok){
    //If we go from ID_0 to ID_1, compute also for ID_0
    if(ID_subset == 1){
      onlineManager->compute_onlineOpe(cloud, 0);
    }

    onlineManager->compute_onlineOpe(cloud, ID_subset);
  }

  //---------------------------
}
bool Offline::select_rangeLimit(Cloud* cloud, int& ID_subset){
  Subset* subset_first = sceneManager->get_subset(cloud, 0);
  Subset* subset_last = sceneManager->get_subset(cloud, cloud->nb_subset-1);
  //---------------------------

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
  //---------------------------

  if(player_flag_1s){
    Cloud* cloud = sceneManager->get_cloud_selected();
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
void Offline::player_pause(){
  this->player_isrunning = false;
  this->player_ispaused = true;
  //---------------------------

  timerManager->stop();

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
  Saver saverManager;
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    saverManager.save_subset(subset, "ply", player_saveas);
  }

  //---------------------------
}
void Offline::player_selectDirSave(){
  //---------------------------

  string path;
  Pather pathManager;
  pathManager.selectDirectory(path);

  this->player_saveas = path + "/";

  //---------------------------
}
void Offline::player_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->player_frequency = value;

  //---------------------------
}