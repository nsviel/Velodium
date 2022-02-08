#include "Interfacing.h"

#include "../Module_node.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Engine_node.h"
#include "../../Load/Pather.h"
#include "../../Load/Saver.h"

#include "../../Specific/fct_watcher.h"
#include "../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
Interfacing::Interfacing(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();

  this->renderManager = node_engine->get_renderManager();
  this->sceneManager = new Scene();
  this->saverManager = new Saver();
  this->pathManager = new Pather();

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->path_frame = path_dir + "frame/";
  this->path_predi = path_dir + "prediction/";
  this->path_grThr = path_dir + "groundtruth/";
  this->path_image = path_dir + "image/";

  this->save_frame_ID = 0;
  this->save_frame_max = 20;
  this->save_image_ID = 0;
  this->save_image_max = 20;

  this->thread_predi_ON = false;
  this->thread_grThr_ON = false;
  this->flag_newPred = false;
  this->flag_newGrTh = false;

  this->clean_directories();

  //---------------------------
}
Interfacing::~Interfacing(){}

//Input: Prediction stuff
void Interfacing::start_dirWatcher(){
  //---------------------------

  this->thread_predi_ON = true;
  this->thread_grThr_ON = true;

  thread_predi = std::thread([&](){
    while(thread_predi_ON){
      watcher_created_file(path_predi, path_predi_file, flag_newPred);
    }
  });
  thread_grThr = std::thread([&](){
    while(thread_grThr_ON){
      watcher_created_file(path_grThr, path_grThr_file, flag_newGrTh);
    }
  });

  thread_predi.detach();
  thread_grThr.detach();

  //---------------------------
}
void Interfacing::stop_dirWatcher(){
  //---------------------------

  this->thread_predi_ON = false;
  this->thread_grThr_ON = false;

  thread_predi.~thread();
  thread_grThr.~thread();

  //---------------------------
}
bool Interfacing::check_prediction(Cloud* cloud){
  //---------------------------

  if(thread_predi_ON && cloud != nullptr){
    //Load json files - predictions
    if(flag_newPred){
      this->parse_obstacle_json(cloud, path_predi_file, "pr");
      flag_newPred = false;
      return true;
    }

    //Load json files - GT
    if(flag_newGrTh){
      this->parse_obstacle_json(cloud, path_grThr_file, "gt");
      flag_newGrTh = false;
      return true;
    }
  }

  //---------------------------
  return false;
}
void Interfacing::parse_obstacle_json(Cloud* cloud, string path, string data){
  if(cloud == nullptr) return;
  //---------------------------

  //Get json name
  std::ifstream ifs(path);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);
  string json_name = obj["frame_id"].asString();

  //For the subset with same name
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->name == json_name){
      //Make stuff
      Obstac* obstacle_gt = &subset->obstacle_gt;
      Obstac* obstacle_pr = &subset->obstacle_pr;
      const Json::Value& json_dete = obj["detections"];

      for (int i = 0; i < json_dete.size(); i++){
        //Obstacle name
        string name = json_dete[i]["name"].asString();

        //Obstacle position
        const Json::Value& json_pos = json_dete[i]["position"];
        vec3 position;
        for (int j=0; j<json_pos.size(); j++){
          position[j] = json_pos[j].asFloat();
        }

        //Obstacle dimension
        const Json::Value& json_dim = json_dete[i]["dimensions"];
        vec3 dimension;
        for (int j=0; j<json_dim.size(); j++){
          dimension[j] = json_dim[j].asFloat();
        }

        //Obstacle heading
        const Json::Value& json_head = json_dete[i]["heading"];
        float heading = json_head.asFloat();

        //Store all data
        if(data == "pr"){
          obstacle_pr->name.push_back(name);
          obstacle_pr->position.push_back(position);
          obstacle_pr->dimension.push_back(dimension);
          obstacle_pr->heading.push_back(heading);
        }
        else if(data == "gt"){
          obstacle_gt->name.push_back(name);
          obstacle_gt->position.push_back(position);
          obstacle_gt->dimension.push_back(dimension);
          obstacle_gt->heading.push_back(heading);
        }
      }

      break;
    }
  }

  //---------------------------
}

//Output: frame & Image saving
void Interfacing::save_image(){
  //---------------------------

  //Save image
  string path = path_image + "image_" + to_string(save_image_ID);
  renderManager->render_screenshot(path);

  //Check number of frame and delete last one
  if(save_image_vec.size() < save_image_max){
    save_image_vec.push_back(path);
    save_image_ID++;
  }else{
    std::remove (save_image_vec[save_image_ID].c_str());
    save_image_vec[save_image_ID] = path;
    save_image_ID++;
  }
  if(save_image_ID >= save_image_max){
    save_image_ID = 0;
  }

  //---------------------------
}
void Interfacing::save_image_path(){
  //---------------------------

  string path;
  Pather pathManager;
  pathManager.selectDirectory(path);

  this->path_image = path + "/";

  //---------------------------
}
void Interfacing::save_frame(Cloud* cloud){
  Subset* subset = sceneManager->get_subset(cloud, cloud->nb_subset-1);
  //---------------------------

  //Save frame
  string path = path_frame + subset->name + ".ply";
  saverManager->save_subset(subset, "ply", path_frame);

  //Check number of frame and delete last one
  if(save_frame_vec.size() < save_frame_max){
    save_frame_vec.push_back(path);
    save_frame_ID++;
  }else{
    std::remove (save_frame_vec[save_frame_ID].c_str());
    save_frame_vec[save_frame_ID] = path;
    save_frame_ID++;
  }
  if(save_frame_ID >= save_frame_max){
    save_frame_ID = 0;
  }

  //---------------------------
}

//Subfunctions
void Interfacing::clean_directories(){
  //---------------------------

  clean_directory_files(path_frame.c_str());
  clean_directory_files(path_predi.c_str());
  clean_directory_files(path_grThr.c_str());

  //---------------------------
}
void Interfacing::select_dir_path(){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_dir;

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

    this->path_dir = path_str + "/";
  }

  //---------------------------
}
