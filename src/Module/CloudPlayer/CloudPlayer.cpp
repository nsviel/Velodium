#include "CloudPlayer.h"

#include "../../Operation/Functions/Heatmap.h"
#include "../../Specific/timer.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Data/Database.h"
#include "../../Engine/OpenGL/Camera.h"
#include "../../Load/Loader.h"

#include <experimental/filesystem>

extern struct Database database;


//Constructor / Destructor
CloudPlayer::CloudPlayer(Camera* camManager){
  //---------------------------

  this->cameraManager = camManager;
  this->sceneManager = new Scene();
  this->heatmapManager = new Heatmap();
  this->timerManager = new Timer();

  this->subset_selected = 0;
  this->frame_max_ID = 0;
  this->frame_max_nb = 0;
  this->frame_ID_ts = 0;
  this->frame_display_range = 0;
  this->frequency = 10;
  this->all_frame_visible = false;
  this->playCloud_isrunning = false;
  this->camera_follow = false;
  this->camera_moved = vec3(0, 0, 0);

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
  if(frame_id >= cloud->nb_subset){
    frame_id = 0;
  }
  if(frame_id < 0){
    frame_id = cloud->nb_subset - 1;
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

      //Camera follow up
      if(camera_follow){
        Frame* frame = &subset->frame;
        Eigen::Vector3d trans_b = frame->trans_b;
        vec3 camPos = cameraManager->get_camPos();

        float x = camPos.x + trans_b(0) - camera_moved.x;
        float y = camPos.y + trans_b(1) - camera_moved.y;
        float z = camPos.z;

        vec3 camPos_new = vec3(x, y, z);

        camera_moved = vec3(trans_b(0), trans_b(1), 0);
        cameraManager->set_camPos(camPos_new);
      }

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
  this->playCloud_isrunning = true;
  //---------------------------

  if(timerManager->isRunning() == false){
    //Set timer parameter
    float increment = (1 / (float)frequency) * 1000;
    timerManager->setFunc([&](){
      if(sceneManager->is_atLeastOnecloud()){
        subset_selected++;
        Cloud* cloud = database.cloud_selected;
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
  this->playCloud_isrunning = false;
  //---------------------------

  timerManager->stop();

  //---------------------------
}
void CloudPlayer::playCloud_stop(){
  this->playCloud_isrunning = false;
  //---------------------------

  timerManager->stop();
  subset_selected = 0;
  this->select_byFrameID(database.cloud_selected, subset_selected);

  //---------------------------
}
void CloudPlayer::playCloud_save(Cloud* cloud){
  Loader loaderManager;
  //---------------------------

  //Save each subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];
    loaderManager.save_subset(subset, "ply", saveas);
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
