#include "GPS.h"

#include "../Interface_node.h"
#include "../Local/Filemanager.h"

#include "../../../Engine/Scene/Configuration.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Engine_node.h"

#include "../../../Specific/fct_watcher.h"
#include "../../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
GPS::GPS(Interface_node* node){
  //---------------------------

  Engine_node* node_engine = node->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();
  Filemanager* fileManager = node->get_fileManager();

  this->sceneManager = node_engine->get_sceneManager();

  this->path_gps = fileManager->get_path_data_dir() + "gps/";
  this->with_gps = configManager->parse_json_b("interface", "with_gps");
  this->is_gps = false;

  //---------------------------
}
GPS::~GPS(){}

//Input: GPS position
void GPS::runtime_gps(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(with_gps && cloud != nullptr){
    vector<string> path_vec = list_allPaths(path_gps);

    for(int i=0; i<path_vec.size(); i++){
      string path = path_vec[i];
      string format = get_file_format(path);

      if(format == "gps"){
        this->parse_json_gps(path);
        this->is_gps = true;
      }
    }
  }

  //---------------------------
}

//Subfunctions
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
