#include "Prediction.h"
#include "Filemanager.h"

#include "../Module_obstacle.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"

#include "../../../Interface/File/Directory.h"
#include "../../../Interface/File/Info.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <stdio.h>


//Constructor / Destructor
Prediction::Prediction(Module_obstacle* module){
  //---------------------------

  Node_engine* node_engine = module->get_node_engine();
  Configuration* configManager = module->get_configManager();

  this->fileManager = module->get_fileManager();
  this->sceneManager = node_engine->get_sceneManager();

  this->is_new_pred = false;
  this->with_delete_pred_file = false;

  //---------------------------
}
Prediction::~Prediction(){}

//Main functions
bool* Prediction::runtime_prediction(){
  //---------------------------

  // Retrieve all new pred files
  string path_predi = fileManager->get_path_predi();
  vector<string> path_vec = list_all_path(path_predi);

  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    string format = get_format_from_filename(path);

    if(format == ".json"){
      this->compute_prediction(path);
    }
  }

  //---------------------------
  return &is_new_pred;
}

//Subfunctions
void Prediction::compute_prediction(string path){
  Cloud* cloud = sceneManager->get_selected_cloud();
  if(cloud == nullptr){
    return;
  }
  //---------------------------

  //Retrieve prediction frame ID
  int frame_ID = parse_frame_ID(path);

  //For the subset with same name
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID == frame_ID){
      this->parse_json_prediction(subset, path);
      this->remove_prediction_file(path);
      this->is_new_pred = true;

      string log = "Prediction - file " + path + " parsed to " + subset->name;
      console.AddLog("ok", log);
      break;
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
        break;
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
void Prediction::remove_prediction_file(string path){
  //---------------------------

  remove(path.c_str());

  //---------------------------
}

//JSON parsers
void Prediction::parse_json_groundTruth(Subset* subset, string path_file){
  Data_pred* detection_gt = &subset->detection;
  //---------------------------

  //Reset all values
  detection_gt->name.clear();
  detection_gt->position.clear();
  detection_gt->dimension.clear();
  detection_gt->heading.clear();

  //Parse ground truth json file
  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  //For each hierarchical set
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
    detection_gt->name.push_back(name);
    detection_gt->position.push_back(position);
    detection_gt->dimension.push_back(dimension);
    detection_gt->heading.push_back(heading);
  }

  //---------------------------
}
void Prediction::parse_json_prediction(Subset* subset, string path_file){
  //---------------------------

  // Clear old data
  subset->detection.name.clear();
  subset->detection.position.clear();
  subset->detection.dimension.clear();
  subset->detection.heading.clear();

  //Parse prediction json file
  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  //For each hierarchical set
  const Json::Value& json = obj["detections"];
  for(int i=0; i<json.size(); i++){
    // Name
    string name = json[i]["name"].asString();

    // Position
    const Json::Value& json_pos = json[i]["position"];
    vec3 position;
    for (int j=0; j<json_pos.size(); j++){
      position[j] = json_pos[j].asFloat();
    }

    // Dimension
    const Json::Value& json_dim = json[i]["dimensions"];
    vec3 dimension;
    for (int j=0; j<json_dim.size(); j++){
      dimension[j] = json_dim[j].asFloat();
    }

    // Heading
    const Json::Value& json_head = json[i]["heading"];
    float heading = json_head.asFloat();

    //Store all data
    subset->detection.name.push_back(name);
    subset->detection.position.push_back(position);
    subset->detection.dimension.push_back(dimension);
    subset->detection.heading.push_back(heading);
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
