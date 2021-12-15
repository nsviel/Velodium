#include "Configuration.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>

Config configuration;

//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->configFilePath = "config.ini";
  this->jsonPath = "config.json";

  //---------------------------
}
Configuration::~Configuration(){}

//Main functions
void Configuration::save_configuration(){
  //---------------------------

  this->create_configFile();
  this->write_configData();

  //---------------------------
}

//Subfunctions
void Configuration::initialize_configStruct(){
  //---------------------------

  //Windows
  strcpy(configuration.WINDOW_Title, "Velodium");
  configuration.WINDOW_InitResWidth = 1024;
  configuration.WINDOW_InitResHeight = 500;
  configuration.WINDOW_Resolution = 4.0f/3.0f;
  configuration.WINDOW_BckgColor = 0.86f;
  configuration.WINDOW_MultiSample = 4;

  //GUI
  configuration.GUI_LeftPanel_width = 220;
  configuration.GUI_TopPanel_height = 18;
  configuration.GUI_BotPanel_height = 100;
  configuration.GUI_LeftPanel_mid = 200;

  //OpenGL
  configuration.GL_ForceVersion = false;
  configuration.GL_WaitForEvent = false;
  configuration.VERBOSE_shader = false;
  configuration.VERBOSE_coreGL = false;

  //Parameters
  strcpy(configuration.INIT_DefaultDirPath, "../media/");
  configuration.TRANSFORM_Trans = 0.01;
  configuration.TRANSFORM_Rot = 5; //Degree
  configuration.CLOUD_movement = true;

  //Camera
  configuration.CAM_FOV = 65.0f;
  configuration.CAM_InitialPos = 5.0f;
  configuration.CAM_NearClip = 0.0001f;
  configuration.CAM_FarClip = 1000.0f;
  configuration.CAM_MouseSpeed = 0.003f;
  configuration.CAM_MoveSpeed = 3.0f;
  configuration.CAM_ZoomSpeed = 0.1f;

  //---------------------------
}
void Configuration::create_configFile(){
  //---------------------------

  remove(configFilePath.c_str());
  std::ofstream file(configFilePath.c_str());
  file.close();

  //---------------------------
}
void Configuration::write_configData(){
  FILE* file = fopen(configFilePath.c_str(), "w");
  //---------------------------

  //If can't open the file
  if(file == NULL){
    fprintf(stderr, "\nError opend file\n");
    exit (1);
  }

  // write struct to file
  fwrite (&configuration, sizeof(struct Config), 1, file);

  //---------------------------
  fclose(file);
}
void Configuration::read_configData(){
  FILE* file = fopen(configFilePath.c_str(), "r");
  //---------------------------

  //If can't open the file
  if(file == NULL){
    fprintf(stderr, "\nError opening file\n");
    exit (1);
  }

  // read file contents till end of file
  size_t size = fread(&configuration, sizeof(struct Config), 1, file);

  //---------------------------
  fclose(file);
}
bool Configuration::is_file_exist(string fileName){
  std::ifstream infile(fileName.c_str());
  return infile.good();
}


void Configuration::make_configuration(){
  bool exist = is_file_exist(jsonPath.c_str());
  //---------------------------

  if(exist == false){
    this->create_jsonfile();
  }

  //---------------------------
}
void Configuration::create_jsonfile(){
  //---------------------------

  std::ifstream ifs(jsonPath);
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
    window["nb_multisample"] = 4;
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
    root["opengl"] = gl;

    //Parameters
    Json::Value transf;
    transf["path_media"] = "../media/";
    transf["cloud_translation"] = 0.01;
    transf["cloud_rotation"] = 5; //Degree
    transf["cloud_movement"] = true;
    root["transformation"] = transf;

    //Camera
    Json::Value camera;
    camera["fov"] = 65.0f;
    camera["initial_pos"] = 5.0f;
    camera["clip_near"] = 0.0001f;
    camera["clip_far"] = 1000.0f;
    camera["speed_mouse"] = 0.003f;
    camera["speed_move"] = 3.0f;
    camera["speed_zoom"] = 0.1f;
    root["camera"] = camera;

    //Write all above stuff
    Json::StyledWriter writer;
    string strWrite = writer.write(root);
    ofstream ofs;
    ofs.open(jsonPath);
    ofs << strWrite;
    ofs.close();
  }
  ifs.close();

  //---------------------------
}
string Configuration::parse_json_string(string field, string value){
  //---------------------------

  std::ifstream ifs(jsonPath);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  string truc = json_field[value].asString();

  //---------------------------
  return truc;
}
float Configuration::parse_json_float(string field, string value){
  //---------------------------

  std::ifstream ifs(jsonPath);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  float truc = json_field[value].asFloat();

  //---------------------------
  return truc;
}
int Configuration::parse_json_int(string field, string value){
  //---------------------------

  std::ifstream ifs(jsonPath);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  int truc = json_field[value].asInt();

  //---------------------------
  return truc;
}
bool Configuration::parse_json_bool(string field, string value){
  //---------------------------

  std::ifstream ifs(jsonPath);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];
  bool truc = json_field[value].asBool();

  //---------------------------
  return truc;
}
