//Default configuration is always build, but other config file are prioritize.
//Thus, if a value is not in a specific config file, the value will be taken in the default config file

#include "Configuration.h"

#include "../../Specific/fct_system.h"


//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->path_config_default = get_absolutePath_build() + "../media/engine/config/config_default.json";
  this->path_config_capture = get_absolutePath_build() + "../media/engine/config/config_capture.json";
  this->path_config_ai = get_absolutePath_build() + "../media/engine/config/config_ai.json";
  this->path_config_server = get_absolutePath_build() + "../media/engine/config/config_server.json";
  this->path_config = path_config_default;
  this->config = 0;

  //---------------------------
}
Configuration::~Configuration(){}

//Main functions
void Configuration::make_configuration(){
  //---------------------------

  //Check if config file exists
  bool is_default = is_file_exist(path_config_default.c_str());
  bool is_config = is_file_exist(path_config.c_str());

  //if not, create them
  if(is_default == false){
    this->create_jsonfile(path_config_default);

    bool is_default = is_file_exist(path_config_default.c_str());
    if(is_default){
      cout<<"[\033[1;32mOK\033[0m] Default configuration file (\033[1;32m"+  path_config_default +"\033[0m) created"<<endl;
    }else{
      cout<<"[\033[1;31merror\033[0m] Default configuration file creation (\033[1;31m"+  path_config_default +"\033[0m) problem"<<endl;
    }
  }
  if(is_config == false && path_config != path_config_default){
    this->create_jsonfile(path_config);

    is_config = is_file_exist(path_config.c_str());
    if(is_config){
      cout<<"[\033[1;32mOK\033[0m] Specific configuration file (\033[1;32m"+  path_config +"\033[0m) created"<<endl;
    }else{
      cout<<"[\033[1;31merror\033[0m] Specific configuration file creation (\033[1;31m"+  path_config +"\033[0m) problem"<<endl;
    }
  }

  //---------------------------
}
void Configuration::make_preconfig(int config){
  //---------------------------

  switch(config){
    case 0:{ // Default
      this->path_config = path_config_default;
      this->config = 0;
      this->make_configuration();
      break;
    }
    case 1:{ // Capture
      this->path_config = path_config_capture;
      this->config = 1;
      this->make_configuration();
      break;
    }
    case 2:{ // AI
      this->path_config = path_config_ai;
      this->config = 2;
      this->make_configuration();
      break;
    }
    case 3:{ // Server
      this->path_config = path_config_server;
      this->config = 3;
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
  param["path_data"] = "../media/data/";
  param["open_mode"] = "cloud";
  param["save_mode"] = "cloud";
  param["cloud_translation"] = 0.01;
  param["cloud_rotation"] = 5; //Degree
  param["cloud_movement"] = true;
  param["point_size"] = 2;
  param["clean_directories"] = true;
  param["check_directories"] = true;
  param["color_mode"] = 0;
  param["color_intensity_min"] = 0;
  param["color_intensity_max"] = 255;
  param["filter_cylinder_rmin"] = 5;
  param["filter_cylinder_rmax"] = 30;
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

  //Network
  Json::Value network;
  network["with_http_demon"] = true;
  network["http_port"] = 8888;

  network["mqtt_sncf_port"] = 1883;
  network["mqtt_sncf_dest"] = "mqtt_sncf_broker";
  network["mqtt_sncf_client"] = "ai_module";
  network["mqtt_sncf_topic"] = "obstacle";

  network["mqtt_local_port"] = 1883;
  network["mqtt_local_dest"] = "localhost";
  network["mqtt_local_client"] = "velodium";
  network["mqtt_local_topic"] = "lidar";

  root["network"] = network;

  //Wallet
  Json::Value wallet;
  wallet["localhost"] = "127.0.0.1";
  wallet["mqtt_sncf_broker"] = "127.0.0.1";

  wallet["mine_ordi_nathan"] = "10.201.20.106";
  wallet["mine_ordi_louis"] = "10.201.20.110";
  wallet["mine_server"] = "10.201.224.13";
  wallet["portable_nathan_home"] = "192.168.1.27";
  wallet["portable_nathan_mine"] = "192.168.153.147";

  root["wallet"] = wallet;

  //Glyph
  Json::Value glyph;
  glyph["aabb_visibility"] = true;
  glyph["grid_visibility"] = true;
  glyph["normal_visibility"] = false;
  glyph["trajectory_visibility"] = false;
  root["glyph"] = glyph;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = true;
  module["with_cylinder_cleaning"] = true;
  module["player_mode"] = "player";
  root["module"] = module;

  //Interface
  Json::Value interface;
  interface["path_save_frame"] = "../../Hubium/data/frame/";
  interface["path_save_image"] = "../../Hubium/data/image/";
  interface["lidar_model"] = "velodyne_vlp64";
  interface["capture_port"] = 2370;
  interface["with_capture"] = false;
  interface["with_prediction"] = false;
  interface["with_save_image"] = false;
  interface["with_save_frame"] = false;
  interface["with_remove_lastSubset"] = false;
  interface["nb_save_frame"] = 20;
  interface["nb_save_image"] = 1;
  root["interface"] = interface;

  //---------------------------
}
void Configuration::preconf_capture(Json::Value& root){
  //---------------------------

  //Module
  Json::Value module;
  module["with_slam"] = false;
  module["with_camera_follow"] = false;
  module["with_cylinder_cleaning"] = false;
  module["player_mode"] = "capture";
  root["module"] = module;

  //Parameters
  Json::Value param;
  param["open_mode"] = "frame";
  param["save_mode"] = "saved_frame";
  param["color_mode"] = 1;
  param["color_intensity_min"] = 0;
  param["color_intensity_max"] = 100;
  root["parameter"] = param;

  //Interface
  Json::Value interface;
  interface["lidar_model"] = "velodyne_vlp16";
  interface["capture_port"] = 2369;
  interface["with_capture"] = true;
  interface["with_prediction"] = true;
  interface["with_save_image"] = false;
  interface["with_save_frame"] = true;
  interface["with_remove_lastSubset"] = false;
  interface["nb_save_frame"] = 200000;
  root["interface"] = interface;

  //---------------------------
}
void Configuration::preconf_ai(Json::Value& root){
  //---------------------------

  //Parameters
  Json::Value param;
  param["path_data"] = "../../data/";
  root["parameter"] = param;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = true;
  module["with_cylinder_cleaning"] = false;
  root["module"] = module;

  //Glyph
  Json::Value glyph;
  glyph["aabb_visibility"] = false;
  glyph["trajectory_visibility"] = true;
  root["glyph"] = glyph;

  //Interface
  Json::Value interface;
  interface["lidar_model"] = "velodyne_vlp64";
  interface["with_prediction"] = true;
  interface["with_save_image"] = false;
  interface["with_save_frame"] = true;
  interface["with_remove_lastSubset"] = false;
  interface["nb_save_image"] = 1;
  root["interface"] = interface;

  //---------------------------
}
void Configuration::preconf_server(Json::Value& root){
  //---------------------------

  //Parameters
  Json::Value param;
  param["path_data"] = "../../data/";
  param["filter_cylinder_rmin"] = 0.5;
  param["filter_cylinder_rmax"] = 30;
  param["color_mode"] = 2; //Heatmap
  root["parameter"] = param;

  //Glyph
  Json::Value glyph;
  glyph["aabb_visibility"] = false;
  root["glyph"] = glyph;

  //Module
  Json::Value module;
  module["with_slam"] = true;
  module["with_camera_follow"] = true;
  module["with_cylinder_cleaning"] = true;
  root["module"] = module;

  //Interface
  Json::Value interface;
  interface["lidar_model"] = "velodyne_vlp16";
  interface["with_capture"] = true;
  interface["with_prediction"] = true;
  interface["with_save_image"] = true;
  interface["with_save_frame"] = true;
  interface["with_remove_lastSubset"] = true;
  root["interface"] = interface;


  //---------------------------
}

//json stuff
void Configuration::create_jsonfile(string path_file){
  //---------------------------

  std::ifstream ifs(path_file);
  Json::Reader reader;
  Json::Value root;

  if (reader.parse(ifs, root) == false){
    this->preconf_default(root);

    if(path_file == path_config_default){
      this->preconf_default(root);
    }else if(path_file == path_config_ai){
      this->preconf_ai(root);
    }else if(path_file == path_config_capture){
      this->preconf_capture(root);
    }else if(path_file == path_config_server){
      this->preconf_server(root);
    }

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

//Parser functions
string Configuration::parse_json_s(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  const Json::Value& json_field = root[field];

  //If value exist ok, but if not check default config file
  string result;
  if(root.isMember(field) && json_field.isMember(value)){
    result = json_field[value].asString();
  }else{
    std::ifstream ifs_default(path_config_default);
    Json::Reader reader;
    Json::Value root;
    reader.parse(ifs_default, root);

    const Json::Value& json_field = root[field];

    //Check finally if default has it, if not say it
    if(json_field.isMember(value)){
      result = json_field[value].asString();
    }else{
      if(is_file_exist(path_config.c_str()) == false){
        cout<<"Config problem: the file \e[32m"<<path_config<<"\e[0m does not exists !"<<endl;
      }else{
        cout<<"Config problem: the value \e[32m"<<value<<"\e[0m in field \e[32m"<<field<<"\e[0m does not exists !"<<endl;
      }
    }

    ifs_default.close();
  }

  ifs.close();

  //---------------------------
  return result;
}
map<string, string> Configuration::parse_json_dict(string field){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);

  Json::Value& json_field = root[field];

  //If value exist ok, but if not check default config file
  map<string, string> dict;
  if(root.isMember(field)){
    for(Json::Value::const_iterator itr=json_field.begin(); itr!=json_field.end(); itr++){
      string field_itr = itr.key().asString();
      string result = json_field[field_itr].asString();
      dict.insert ( std::pair<string, string>(field_itr, result) );
    }
  }else{
    std::ifstream ifs_default(path_config_default);
    Json::Reader reader;
    Json::Value root;
    reader.parse(ifs_default, root);

    const Json::Value& json_field = root[field];

    //Check finally if default has it, if not say it
    if(root.isMember(field)){
      for(Json::Value::const_iterator itr=json_field.begin(); itr!=json_field.end(); itr++){
        string field_itr = itr.key().asString();
        string result = json_field[field_itr].asString();
        dict.insert ( std::pair<string, string>(field_itr, result) );
      }
    }else{
      if(is_file_exist(path_config.c_str()) == false){
        cout<<"Config problem: the file \e[32m"<<path_config<<"\e[0m does not exists !"<<endl;
      }else{
        cout<<"Config problem: the field \e[32m"<<field<<"\e[0m does not exists !"<<endl;
      }
    }

    ifs_default.close();
  }

  ifs.close();

  //---------------------------
  return dict;
}
float Configuration::parse_json_f(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);
  const Json::Value& json_field = root[field];

  //If value exist ok, but if not check default config file
  float result;
  if(root.isMember(field) && json_field.isMember(value)){
    result = json_field[value].asFloat();
  }else{
    std::ifstream ifs_default(path_config_default);
    Json::Reader reader;
    Json::Value root;
    reader.parse(ifs_default, root);

    const Json::Value& json_field = root[field];

    //Check finally if default has it, if not say it
    if(json_field.isMember(value)){
      result = json_field[value].asFloat();
    }else{
      if(is_file_exist(path_config.c_str()) == false){
        cout<<"Config problem: the file \e[32m"<<path_config<<"\e[0m does not exists !"<<endl;
      }else{
        cout<<"Config problem: the value \e[32m"<<value<<"\e[0m in field \e[32m"<<field<<"\e[0m does not exists !"<<endl;
      }
    }

    ifs_default.close();
  }

  ifs.close();

  //---------------------------
  return result;
}
int Configuration::parse_json_i(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);
  const Json::Value& json_field = root[field];

  //If value exist ok, but if not check default config file
  int result;
  if(root.isMember(field) && json_field.isMember(value)){
    result = json_field[value].asInt();
  }else{
    std::ifstream ifs_default(path_config_default);
    Json::Reader reader;
    Json::Value root;
    reader.parse(ifs_default, root);

    const Json::Value& json_field = root[field];

    //Check finally if default has it, if not say it
    if(json_field.isMember(value)){
      result = json_field[value].asInt();
    }else{
      if(is_file_exist(path_config.c_str()) == false){
        cout<<"Config problem: the file \e[32m"<<path_config<<"\e[0m does not exists !"<<endl;
      }else{
        cout<<"Config problem: the value \e[32m"<<value<<"\e[0m in field \e[32m"<<field<<"\e[0m does not exists !"<<endl;
      }
    }

    ifs_default.close();
  }

  ifs.close();

  //---------------------------
  return result;
}
bool Configuration::parse_json_b(string field, string value){
  //---------------------------

  std::ifstream ifs(path_config);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);
  const Json::Value& json_field = root[field];

  //If value exist ok, but if not check default config file
  bool result;
  if(root.isMember(field) && json_field.isMember(value)){
    result = json_field[value].asBool();
  }else{
    std::ifstream ifs_default(path_config_default);
    Json::Reader reader;
    Json::Value root;
    reader.parse(ifs_default, root);

    const Json::Value& json_field = root[field];

    //Check finally if default has it, if not say it
    if(json_field.isMember(value)){
      result = json_field[value].asBool();
    }else{
      if(is_file_exist(path_config.c_str()) == false){
        cout<<"Config problem: the file \e[32m"<<path_config<<"\e[0m does not exists !"<<endl;
      }else{
        cout<<"Config problem: the value \e[32m"<<value<<"\e[0m in field \e[32m"<<field<<"\e[0m does not exists !"<<endl;
      }
    }

    ifs_default.close();
  }

  ifs.close();

  //---------------------------
  return result;
}
bool Configuration::is_file_exist(string fileName){
  std::ifstream infile(fileName.c_str());
  return infile.good();
}
