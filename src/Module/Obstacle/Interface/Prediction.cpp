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

//Main functions
bool Prediction::runtime_prediction(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  bool is_prediction = false;
  //---------------------------

  if(thread_predi_ON || thread_grThr_ON){
    if(cloud != nullptr){
      //Load json files - predictions
      if(flag_newPred){
        this->compute_prediction(cloud, path_predi_file);
        this->flag_newPred = false;
        is_prediction = true;
      }

      //Load json files - GT
      if(flag_newGrTh){
        this->compute_groundTruth(cloud, path_grThr_file);
        this->flag_newGrTh = false;
        is_prediction = true;
      }
    }
  }

  //---------------------------
  return is_prediction;
}
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
void Prediction::compute_prediction(Cloud* cloud, string path_file){
  if(cloud == nullptr) return;
  //---------------------------

  //Retrieve prediction frame ID
  int frame_ID = parse_frame_ID(path_file);

  //For the subset with same name
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID == frame_ID){
      this->parse_json_prediction(subset, path_file);
    }
  }

  //---------------------------
}
void Prediction::compute_prediction(Cloud* cloud, vector<string> path_vec){
  if(cloud == nullptr) return;
  //---------------------------

  for(int i=0; i<path_vec.size(); i++){
    string path_file = path_vec[i];

    //Retrieve prediction frame ID
    int frame_ID = parse_frame_ID(path_file);

    //For the subset with same name
    for(int j=0; j<cloud->subset.size(); j++){
      Subset* subset = sceneManager->get_subset(cloud, j);

      if(subset->ID == frame_ID){
        this->parse_json_prediction(subset, path_file);
      }
    }
  }

  //---------------------------
}
void Prediction::compute_groundTruth(Cloud* cloud, string path_file){
  if(cloud == nullptr) return;
  //---------------------------

  //Retrieve prediction frame ID
  int frame_ID = parse_frame_ID(path_file);

  //For the subset with same name
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID == frame_ID){
      this->parse_json_groundTruth(subset, path_file);
    }
  }

  //---------------------------
}
void Prediction::compute_groundTruth(Cloud* cloud, vector<string> path_vec){
  if(cloud == nullptr) return;
  //---------------------------

  for(int i=0; i<path_vec.size(); i++){
    string path_file = path_vec[i];

    //Retrieve prediction frame ID
    int frame_ID = parse_frame_ID(path_file);

    //For the subset with same name
    for(int j=0; j<cloud->subset.size(); j++){
      Subset* subset = sceneManager->get_subset(cloud, j);

      if(subset->ID == frame_ID){
        this->parse_json_groundTruth(subset, path_file);
      }
    }
  }

  //---------------------------
}

//JSON parsers
void Prediction::parse_json_groundTruth(Subset* subset, string path_file){
  //---------------------------

  //Parse ground truth json file
  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  //For each hierarchical set
  Obstac* obstacle_gt = &subset->obstacle_gt;
  const Json::Value& json = obj["detections"];

  for (int i = 0; i < json.size(); i++){
    //Obstacle name
    string name = json[i]["name"].asString();

    //Obstacle position
    const Json::Value& json_pos = json[i]["position"];
    vec3 position;
    for (int j=0; j<json_pos.size(); j++){
      position[j] = json_pos[j].asFloat();
    }

    //Obstacle dimension
    const Json::Value& json_dim = json[i]["dimensions"];
    vec3 dimension;
    for (int j=0; j<json_dim.size(); j++){
      dimension[j] = json_dim[j].asFloat();
    }

    //Obstacle heading
    const Json::Value& json_head = json[i]["heading"];
    float heading = json_head.asFloat();

    //Store all data
    obstacle_gt->name.push_back(name);
    obstacle_gt->position.push_back(position);
    obstacle_gt->dimension.push_back(dimension);
    obstacle_gt->heading.push_back(heading);
  }

  //---------------------------
}
void Prediction::parse_json_prediction(Subset* subset, string path_file){
  //---------------------------

  //Parse prediction json file
  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  //For each hierarchical set
  Obstac* obstacle_pr = &subset->obstacle_pr;
  const Json::Value& json = obj["detections"];

  for (int i = 0; i < json.size(); i++){
    //Obstacle name
    string name = json[i]["name"].asString();

    //Obstacle position
    const Json::Value& json_pos = json[i]["position"];
    vec3 position;
    for (int j=0; j<json_pos.size(); j++){
      position[j] = json_pos[j].asFloat();
    }

    //Obstacle dimension
    const Json::Value& json_dim = json[i]["dimensions"];
    vec3 dimension;
    for (int j=0; j<json_dim.size(); j++){
      dimension[j] = json_dim[j].asFloat();
    }

    //Obstacle heading
    const Json::Value& json_head = json[i]["heading"];
    float heading = json_head.asFloat();

    //Store all data
    obstacle_pr->name.push_back(name);
    obstacle_pr->position.push_back(position);
    obstacle_pr->dimension.push_back(dimension);
    obstacle_pr->heading.push_back(heading);
  }

  //---------------------------
}
int Prediction::parse_frame_ID(string path_file){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);
  int frame_ID = obj["frame_id"].asInt();

  //---------------------------
  return frame_ID;
}
