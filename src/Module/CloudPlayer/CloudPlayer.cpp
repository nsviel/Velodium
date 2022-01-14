#include "CloudPlayer.h"

#include "../../Specific/timer.h"
#include "../../Engine/Scene.h"
#include "../../Load/Saver.h"

#include <experimental/filesystem>


//Constructor / Destructor
CloudPlayer::CloudPlayer(){
  //---------------------------

  this->sceneManager = new Scene();
  this->timerManager = new Timer();

  this->subset_selected = 0;
  this->frame_max_ID = 0;
  this->frame_max_nb = 0;
  this->frame_ID_ts = 0;
  this->frame_display_range = 0;
  this->frequency = 10;
  this->player_isrunning = false;
  this->with_restart = false;

  //Get absolute executable location
  string absPath = std::experimental::filesystem::current_path();
  this->saveas = absPath + "/../media/data/";

  //---------------------------
}
CloudPlayer::~CloudPlayer(){}

//Main function
void CloudPlayer::select_byFrameID(Cloud* cloud, int frame_id){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  //---------------------------

  //If frame desired ID is superior to the number of subset restart it
  if(with_restart){
    if(frame_id >= cloud->nb_subset){
      frame_id = 0;
    }

    if(frame_id < 0){
      frame_id = cloud->nb_subset - 1;
    }
  }
  if(!with_restart){
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

    //If several frame displayed
    if(i >= frame_id - frame_display_range && i < frame_id && i >= 0){
      subset->visibility = true;
    }
    //Selected frame
    else if(i == frame_id){
      subset->visibility = true;
      cloud->subset_selected = i;

      if(subset->ts.size() != 0){
        frame_ID_ts = subset->ts[0];
      }

      /*//If camera follow up option activated
      if(with_camera_follow){
        this->camera_followUp(subset);
      }

      //If with slam option is activated
      if(with_slam && player_isrunning){
        slamManager->compute_slam_online(cloud, i);
      }*/

    }
    //All other frames
    else{
      subset->visibility = false;
    }
  }

  //---------------------------
  this->subset_selected = frame_id;
}
void CloudPlayer::update_frame_ID(Cloud* cloud){
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
void CloudPlayer::supress_firstSubset(Cloud* cloud){
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

void CloudPlayer::playCloud_start(){
  this->player_isrunning = true;
  //---------------------------

  if(timerManager->isRunning() == false){
    //Set timer parameter
    float increment = (1 / (float)frequency) * 1000;
    timerManager->setFunc([&](){
      if(sceneManager->is_atLeastOnecloud()){
        subset_selected++;
        Cloud* cloud = sceneManager->get_cloud_selected();
        this->select_byFrameID(cloud, subset_selected);
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
void CloudPlayer::playCloud_pause(){
  this->player_isrunning = false;
  //---------------------------

  timerManager->stop();

  //---------------------------
}
void CloudPlayer::playCloud_stop(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  this->player_isrunning = false;
  //---------------------------

  timerManager->stop();
  subset_selected = 0;
  this->select_byFrameID(cloud, subset_selected);

  //---------------------------
}
void CloudPlayer::playCloud_save(Cloud* cloud){
  Saver saverManager;
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];
    saverManager.save_subset(subset, "ply", saveas);
  }

  //---------------------------
}
void CloudPlayer::playCloud_selectDirSave(){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + saveas;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
    }

    this->saveas = path_str + "/";
  }

  //---------------------------
}
void CloudPlayer::play_setFrequency(int value){
  //---------------------------

  timerManager->stop();
  this->frequency = value;

  //---------------------------
}
