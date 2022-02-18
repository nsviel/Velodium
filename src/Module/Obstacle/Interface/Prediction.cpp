#include "Prediction.h"

#include "../../../Engine/Scene.h"

#include "../../../Specific/fct_watcher.h"
#include "../../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
Prediction::Prediction(){
  //---------------------------

  this->sceneManager = new Scene();

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->path_predi = path_dir + "prediction/";
  this->path_grThr = path_dir + "groundtruth/";

  this->thread_predi_ON = false;
  this->thread_grThr_ON = false;
  this->flag_newPred = false;
  this->flag_newGrTh = false;
  this->is_whatching = false;

  //---------------------------
}
Prediction::~Prediction(){}

//Watchers
void Prediction::start_watcher_prediction(){
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

  this->is_whatching = true;

  //---------------------------
}
void Prediction::stop_watcher_prediction(){
  //---------------------------

  this->thread_predi_ON = false;
  this->thread_grThr_ON = false;

  thread_predi.~thread();
  thread_grThr.~thread();

  this->is_whatching = false;

  //---------------------------
}

//Subfunctions
bool Prediction::runtime_prediction(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  bool is_prediction = false;
  //---------------------------

  if(thread_predi_ON || thread_grThr_ON){
    if(cloud != nullptr){
      //Load json files - predictions
      if(flag_newPred){
        this->parse_json_prediction(cloud, path_predi_file, "pr");
        this->flag_newPred = false;
        is_prediction = true;
      }

      //Load json files - GT
      if(flag_newGrTh){
        this->parse_json_prediction(cloud, path_grThr_file, "gt");
        this->flag_newGrTh = false;
        is_prediction = true;
      }
    }
  }

  //---------------------------
  return is_prediction;
}
void Prediction::parse_json_prediction(Cloud* cloud, string path, string data){
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
