#include "Configuration.h"

#include "../../Specific/fct_system.h"


//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->path_default = get_absolutePath_build() + "/../media/engine/config_default.json";
  this->path_WP4_car = get_absolutePath_build() + "/../media/engine/config_WP4.json";
  this->path_WP5_train = get_absolutePath_build() + "/../media/engine/config_WP5.json";
  this->path_config = path_default;
  this->config = 0;

  //---------------------------
}
Configuration::~Configuration(){}

//Main functions
void Configuration::make_configuration(){
  bool is_config = is_file_exist(path_config.c_str());
  //---------------------------

  if(is_config == false){
    this->create_jsonfile();
  }

  //---------------------------
}
void Configuration::make_preconfig(int config){
  //---------------------------

  switch(config){
    case 0:{ //Default
      this->path_config = path_default;
      this->config = 0;
      this->make_configuration();
      break;
    }
    case 1:{ //WP4 auto
      this->path_config = path_WP4_car;
      this->config = 1;
      this->make_configuration();
      break;
    }
    case 2:{ //WP5 train
      this->path_config = path_WP5_train;
      this->config = 2;
      this->make_configuration();
      break;
    }
  }

  //---------------------------
}

//Pred-defined configuration
void Configuration::preconf_default(Json::Value& root){
  //---------------------------

  //OpenGL stuff
  Json::Value window;
  window["title"] = "Velodium";
  window["resolution_width"] = 1024;
  window["resolution_height"] = 500;
  window["resolution_ratio"] = 4.0f/3.0f;
  window["background_color"] = 0.86f;
  window["forceVersion"] = false;
  window["visualization"] = true;
  root["window"] = window;

  //GUI
  Json::Value gui;
  gui["leftPanel_width"] = 220;
  gui["leftPanel_mid"] = 200;
  gui["topPanel_height"] = 18;
  gui["botPanel_height"] = 100;
  root["gui"] = gui;

  //Parameters
  Json::Value param;
  param["path_media"] = "../media/";
  param["cloud_translation"] = 0.01;
  param["cloud_rotation"] = 5; //Degree
  param["cloud_movement"] = true;
  param["point_size"] = 2;
  param["clean_directories"] = true;
  param["check_directories"] = true;
  root["parameter"] = param;

  //Camera
  Json::Value camera;
  camera["fov"] = 65.0f;
  camera["initial_pos"] = 5.0f;
  camera["clip_near"] = 0.1f;
  camera["clip_far"] = 10000.0f;
  camera["speed_mouse"] = 0.003f;
  camera["speed_move"] = 3.0f;
  camera["speed_zoom"] = 0.1f;
  root["camera"] = camera;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = true;
  module["with_cylinder_cleaning"] = true;
  module["with_heatmap"] = true;
  module["with_save_image"] = false;
  module["with_save_frame"] = false;
  module["with_remove_lastSubset"] = false;
  module["with_AI_module"] = false;
  root["module"] = module;

  //---------------------------
}
void Configuration::preconf_WP4_car(Json::Value& root){
  //---------------------------

  //OpenGL stuff
  Json::Value window;
  window["title"] = "Velodium";
  window["resolution_width"] = 1024;
  window["resolution_height"] = 500;
  window["resolution_ratio"] = 4.0f/3.0f;
  window["background_color"] = 0.86f;
  window["forceVersion"] = false;
  window["visualization"] = true;
  root["window"] = window;

  //GUI
  Json::Value gui;
  gui["leftPanel_width"] = 220;
  gui["leftPanel_mid"] = 200;
  gui["topPanel_height"] = 18;
  gui["botPanel_height"] = 100;
  root["gui"] = gui;

  //Parameters
  Json::Value param;
  param["path_media"] = "../media/";
  param["cloud_translation"] = 0.01;
  param["cloud_rotation"] = 5; //Degree
  param["cloud_movement"] = true;
  param["point_size"] = 2;
  param["clean_directories"] = true;
  param["check_directories"] = true;
  root["parameter"] = param;

  //Camera
  Json::Value camera;
  camera["fov"] = 65.0f;
  camera["initial_pos"] = 5.0f;
  camera["clip_near"] = 0.1f;
  camera["clip_far"] = 10000.0f;
  camera["speed_mouse"] = 0.003f;
  camera["speed_move"] = 3.0f;
  camera["speed_zoom"] = 0.1f;
  root["camera"] = camera;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = true;
  module["with_cylinder_cleaning"] = true;
  module["with_heatmap"] = true;
  module["with_save_image"] = true;
  module["with_save_frame"] = true;
  module["with_remove_lastSubset"] = true;
  module["with_AI_module"] = true;
  root["module"] = module;

  //---------------------------
}
void Configuration::preconf_WP5_train(Json::Value& root){
  //---------------------------

  //OpenGL stuff
  Json::Value window;
  window["title"] = "Velodium";
  window["resolution_width"] = 400;
  window["resolution_height"] = 200;
  window["resolution_ratio"] = 4.0f/3.0f;
  window["background_color"] = 0.86f;
  window["forceVersion"] = false;
  window["visualization"] = false;
  root["window"] = window;

  //GUI
  Json::Value gui;
  gui["leftPanel_width"] = 0;
  gui["leftPanel_mid"] = 1;
  gui["topPanel_height"] = 18;
  gui["botPanel_height"] = 300 - 18;
  root["gui"] = gui;

  //Parameters
  Json::Value param;
  param["path_media"] = "../media/";
  param["cloud_translation"] = 0.01;
  param["cloud_rotation"] = 5; //Degree
  param["cloud_movement"] = true;
  param["point_size"] = 2;
  param["clean_directories"] = true;
  param["check_directories"] = true;
  root["parameter"] = param;

  //Camera
  Json::Value camera;
  camera["fov"] = 65.0f;
  camera["initial_pos"] = 5.0f;
  camera["clip_near"] = 0.1f;
  camera["clip_far"] = 10000.0f;
  camera["speed_mouse"] = 0.003f;
  camera["speed_move"] = 3.0f;
  camera["speed_zoom"] = 0.1f;
  root["camera"] = camera;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = false;
  module["with_cylinder_cleaning"] = false;
  module["with_heatmap"] = false;
  module["with_save_image"] = false;
  module["with_save_frame"] = true;
  module["with_remove_lastSubset"] = true;
  module["with_AI_module"] = true;
  root["module"] = module;

  //---------------------------
}

//json stuff
void Configuration::create_jsonfile(){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;

  if (reader.parse(ifs, root) == false){
    this->preconf_default(root);

    switch(this->config){
      case 0:{ //Default
        this->preconf_default(root);
        break;
      }
      case 1:{ //WP4 auto
        this->preconf_WP4_car(root);
        break;
      }
      case 2:{ //WP5 train
        this->preconf_WP5_train(root);
        break;
      }
    }

    //Write all above stuff
    Json::StyledWriter writer;
    string strWrite = writer.write(root);
    ofstream ofs;
    ofs.open(path_config.c_str());
    ofs << strWrite;
    ofs.close();
  }
  ifs.close();

  //---------------------------
}
void Configuration::update_jsonfile(string field, string title, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  json_field[title] == value;

  ifs.close();

  //---------------------------
}
string Configuration::parse_json_s(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  string truc = json_field[value].asString();

  ifs.close();

  //---------------------------
  return truc;
}
float Configuration::parse_json_f(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  float truc = json_field[value].asFloat();

  ifs.close();

  //---------------------------
  return truc;
}
int Configuration::parse_json_i(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  int truc = json_field[value].asInt();

  ifs.close();

  //---------------------------
  return truc;
}
bool Configuration::parse_json_b(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  bool truc = json_field[value].asBool();

  ifs.close();

  //---------------------------
  return truc;
}
bool Configuration::is_file_exist(string fileName){
  std::ifstream infile(fileName.c_str());
  return infile.good();
}
