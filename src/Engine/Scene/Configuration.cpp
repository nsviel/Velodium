#include "Configuration.h"

#include "../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->path_config = get_absolutePath_build() + "/../media/engine/configuration.json";

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

//json stuff
void Configuration::create_jsonfile(){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;

  if (reader.parse(ifs, root) == false){
    //Window
    Json::Value window;
    window["title"] = "Velodium";
    window["resolution_width"] = 1024;
    window["resolution_height"] = 500;
    window["resolution_ratio"] = 4.0f/3.0f;
    window["background_color"] = 0.86f;
    window["activated"] = false;
    root["window"] = window;

    //GUI
    Json::Value gui;
    gui["leftPanel_width"] = 220;
    gui["leftPanel_mid"] = 200;
    gui["topPanel_height"] = 18;
    gui["botPanel_height"] = 100;
    root["gui"] = gui;

    //OpenGL
    Json::Value gl;
    gl["forceVersion"] = false;
    gl["waitForEvent"] = false;
    gl["verbose_shader"] = false;
    gl["verbose_coreGL"] = false;
    gl["nb_multisample"] = 4;
    root["opengl"] = gl;

    //Parameters
    Json::Value param;
    param["path_media"] = "../media/";
    param["cloud_translation"] = 0.01;
    param["cloud_rotation"] = 5; //Degree
    param["cloud_movement"] = true;
    param["point_size"] = 3;
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
    Json::Value online;
    online["with_slam"] = false;
    online["with_camera_follow"] = false;
    online["with_cylinder_cleaning"] = false;
    online["with_heatmap"] = false;
    online["with_save_image"] = false;
    online["with_remove_lastSubset"] = false;
    online["with_keepNframes"] = false;
    online["with_AI_module"] = true;
    root["online"] = online;

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
