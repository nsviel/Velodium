#include "Prediction.h"

#include "Filemanager.h"

#include "../Module_obstacle.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"

#include "../../../Specific/fct_watcher.h"
#include "../../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
Prediction::Prediction(Module_obstacle* module){
  //---------------------------

  Engine_node* node_engine = module->get_node_engine();
  Configuration* configManager = module->get_configManager();
  Filemanager* fileManager = module->get_fileManager();

  this->sceneManager = node_engine->get_sceneManager();

  this->path_predi = fileManager->get_path_data_dir() + "prediction/";
  this->path_grThr = fileManager->get_path_data_dir() + "groundtruth/";
  this->with_prediction = configManager->parse_json_b("interface", "with_prediction");
  this->is_prediction = false;

  //---------------------------
}
Prediction::~Prediction(){}

//Main functions
void Prediction::runtime_prediction(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(with_prediction && cloud != nullptr){
    vector<string> path_vec = list_allPaths(path_predi);

    for(int i=0; i<path_vec.size(); i++){
      string path = path_vec[i];
      string format = get_file_format(path);

      if(format == ".json"){
        this->compute_prediction(cloud, path);
        this->remove_prediction_file(path);
        this->is_prediction = true;
      }
    }
  }

  //---------------------------
}

//Subfunctions
void Prediction::compute_prediction(Cloud* cloud, string path_file){
  //---------------------------

  //Retrieve prediction frame ID
  int frame_ID = parse_frame_ID(path_file);

  //For the subset with same name
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID == frame_ID){
      this->parse_json_prediction(subset, path_file);
      this->is_prediction = true;
      break;
    }
  }

  //---------------------------
}
void Prediction::compute_prediction(string path_dir){
  Cloud* cloud = sceneManager->get_cloud_selected();
  if(cloud == nullptr) return;
  //---------------------------

  //Retrieve prediction frame ID
  vector<string> path_vec = list_allPaths(path_dir);

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

  //remove(path);

  //---------------------------
}

//JSON parsers
void Prediction::parse_json_groundTruth(Subset* subset, string path_file){
  Obstac* obstacle_gt = &subset->obstacle_pr;
  //---------------------------

  //Reset all values
  obstacle_gt->name.clear();
  obstacle_gt->position.clear();
  obstacle_gt->dimension.clear();
  obstacle_gt->heading.clear();

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
    obstacle_gt->name.push_back(name);
    obstacle_gt->position.push_back(position);
    obstacle_gt->dimension.push_back(dimension);
    obstacle_gt->heading.push_back(heading);
  }

  //---------------------------
}
void Prediction::parse_json_prediction(Subset* subset, string path_file){
  if(subset->obstacle_pr.name.size() == 0){
    //---------------------------

    //Parse prediction json file
    std::ifstream ifs(path_file);
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);

    //For each hierarchical set
    const Json::Value& json = obj["detections"];
    for(int i = 0; i < json.size(); i++){
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
      subset->obstacle_pr.name.push_back(name);
      subset->obstacle_pr.position.push_back(position);
      subset->obstacle_pr.dimension.push_back(dimension);
      subset->obstacle_pr.heading.push_back(heading);
    }

    //cout<<subset->name<<" - obstacle: "<<subset->obstacle_pr.name.size()<<endl;

    //---------------------------
  }
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
