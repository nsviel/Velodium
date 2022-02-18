#include "config_opengl.h"

#include "../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
config_opengl::config_opengl(){
  //---------------------------

  this->path_file = "../media/configuration/config_opengl.json";

  //---------------------------
}
config_opengl::~config_opengl(){}

//Main functions
void config_opengl::make_configuration(){
  bool exist = is_file_exist(path_file.c_str());
  //---------------------------

  if(exist == false){
    this->create_jsonfile();
  }

  //---------------------------
}

//json stuff
void config_opengl::create_jsonfile(){
  //---------------------------

  std::ifstream ifs(path_file);
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

    //Write all above stuff
    Json::StyledWriter writer;
    string strWrite = writer.write(root);
    ofstream ofs;
    ofs.open(path_file.c_str());
    ofs << strWrite;
    ofs.close();
  }
  ifs.close();

  //---------------------------
}
void config_opengl::update_jsonfile(string field, string title, string value){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  json_field[title] == value;

  ifs.close();

  //---------------------------
}
string config_opengl::parse_json_s(string field, string value){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  string truc = json_field[value].asString();

  ifs.close();

  //---------------------------
  return truc;
}
float config_opengl::parse_json_f(string field, string value){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  float truc = json_field[value].asFloat();

  ifs.close();

  //---------------------------
  return truc;
}
int config_opengl::parse_json_i(string field, string value){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  int truc = json_field[value].asInt();

  ifs.close();

  //---------------------------
  return truc;
}
bool config_opengl::parse_json_b(string field, string value){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  bool truc = json_field[value].asBool();

  ifs.close();

  //---------------------------
  return truc;
}
bool config_opengl::is_file_exist(string fileName){
  std::ifstream infile(fileName.c_str());
  return infile.good();
}
