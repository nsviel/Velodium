#include "GPS.h"

#include "../../../Engine/Scene/Configuration.h"
#include "../../../Engine/Engine_node.h"

#include "../../../Specific/fct_watcher.h"
#include "../../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
GPS::GPS(Engine_node* node_engine){
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->path_gps = path_dir + "gps/";

  this->thread_gps_ON = false;
  this->is_whatching = false;

  //---------------------------
}
GPS::~GPS(){}

//Input: GPS position
void GPS::start_watcher_gps(){
  //---------------------------

  this->thread_gps_ON = true;

  thread_gps = std::thread([&](){
    while(thread_gps_ON){
      watcher_created_file(path_gps, path_gps_file, flag_newGPS);
    }
  });
  thread_gps.detach();

  this->is_whatching = true;

  //---------------------------
  console.AddLog("sucess", "Watcher - GPS running...");
}
void GPS::stop_watcher_gps(){
  //---------------------------

  this->thread_gps_ON = false;

  thread_gps.~thread();

  this->is_whatching = false;

  //---------------------------
}

//Subfunctions
bool GPS::runtime_gps(){
  bool is_gps = false;
  //---------------------------

  if(thread_gps_ON){
    //Load json files - gps
    if(flag_newGPS){
      this->parse_json_gps(path_gps_file);
      this->flag_newGPS = false;
      is_gps = true;
    }
  }

  //---------------------------
  return is_gps;
}
void GPS::parse_json_gps(string path){
  //---------------------------

  //Get json name
  std::ifstream ifs(path);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  //Make stuff
  const Json::Value& json_pos = obj["position"];
  vec3 position;
  for (int j=0; j<json_pos.size(); j++){
    position[j] = json_pos[j].asFloat();
  }

  //---------------------------
}
