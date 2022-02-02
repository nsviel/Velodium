#include "config_module.h"

#include "../../Specific/fct_system.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
config_module::config_module(){
  //---------------------------

  this->path_file = "../media/configuration/config_module_default.json";

  //---------------------------
}
config_module::~config_module(){}

//Main functions
void config_module::make_configuration(){
  //---------------------------

  bool exist = is_file_exist(path_file.c_str());
  if(exist == false){
    this->create_jsonfile_default();
  }

  //---------------------------
}
void config_module::choose_configuration(int config){
  //---------------------------

  switch (config){
    case 0:{
      path_file = "../media/configuration/config_module_default.json";
      this->make_configuration();
      break;
    }
    case 1:{
      path_file = "../media/configuration/config_module_AI.json";
      this->make_configuration();
      break;
    }
  }

  //---------------------------
}

//json stuff
void config_module::create_jsonfile_default(){
  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;

  if (reader.parse(ifs, root) == false){
    //---------------------------

    //Window
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

    //---------------------------
    //Write all above stuff
    Json::StyledWriter writer;
    string strWrite = writer.write(root);
    ofstream ofs;
    ofs.open(path_file.c_str());
    ofs << strWrite;
    ofs.close();
  }
  ifs.close();
}
void config_module::update_jsonfile(string field, string title, string value){
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
string config_module::parse_json_s(string field, string value){
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
float config_module::parse_json_f(string field, string value){
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
int config_module::parse_json_i(string field, string value){
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
bool config_module::parse_json_b(string field, string value){
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
bool config_module::is_file_exist(string fileName){
  std::ifstream infile(fileName.c_str());
  return infile.good();
}
