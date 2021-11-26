#include "Configuration.h"

Config configuration;

//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->configFilePath = "config.ini";

  //---------------------------
}
Configuration::~Configuration(){}

//Main functions
void Configuration::make_configuration(){
  bool exist = is_file_exist(configFilePath.c_str());
  //---------------------------

  if(exist){
    this->read_configData();
  }else{
    this->initialize_configStruct();
  }

  //---------------------------
}
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
  configuration.WINDOW_InitResHeight = 600;
  configuration.WINDOW_Resolution = 4.0f/3.0f;
  configuration.WINDOW_BckgColor = 0.86f;
  configuration.WINDOW_MultiSample = 4;

  //GUI
  configuration.GUI_LeftPanel_width = 220;
  configuration.GUI_TopPanel_height = 18;
  configuration.GUI_BotPanel_height = 125;
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
