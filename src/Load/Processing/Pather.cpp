#include "Pather.h"

#include "Loader.h"
#include "Saver.h"

#include "../Load_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_system.h"
#include "../../Specific/fct_maths.h"

#include <Eigen/Dense>
#include <set>
#include <string>
#include <filesystem>


//Constructor / destructor
Pather::Pather(Load_node* node_load){
  //---------------------------

  Engine_node* node_engine = node_load->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->saverManager = node_load->get_saveManager();

  //---------------------------
  this->update_configuration();
}
Pather::~Pather(){}

void Pather::update_configuration(){
  //---------------------------

  this->spaceSampling = 0.08f;
  this->nbLineSampling = 1000000;
  this->path_current_dir = get_absolutePath_build() + '/';
  this->open_mode = configManager->parse_json_s("parameter", "open_mode");
  this->save_mode = configManager->parse_json_s("parameter", "save_mode");
  this->path_saved_frame = configManager->parse_json_s("parameter", "path_data")  + "frame";

  //---------------------------
}

//Loading functions
void Pather::loading(){
  //---------------------------

  if(open_mode == "cloud"){
    this->loading_cloud();
  }
  else if(open_mode == "frame"){
    this->loading_frames();
  }

  //---------------------------
}
void Pather::loading_cloud(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading("Cloud loading");

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    loaderManager->load_cloud(path);
  }

  //---------------------------
}
void Pather::loading_frames(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading("Frame loading");

  //Load files
  if(path_vec.size() != 0){
    loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
}
void Pather::loading_directory_frame(string path){
  //---------------------------

  //Supress unwanted line break
  if (path.find('\n')){
    path.erase(std::remove(path.begin(), path.end(), '\n'), path.end());
  }

  //Get all frame path
  vector<string> path_vec = list_allPaths(path);
  fct_sort_alpha_num(path_vec);

  if(path_vec.size() != 0){
    loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
}
void Pather::loading_sampling(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading("Cloud with sampling loading");

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    int size = get_fileSize(path);
    if(size > 1000000){
      this->loading_sampledCloud(path);
    }
  }

  //---------------------------
}
void Pather::loading_sampledCloud(string path){
  /*  int nbLinePart = nbLineSampling;
  int nbPart = 0;
  int lmin = 0;
  int lmax = nbLinePart;
  int size = get_fileSize(path);
  Filter filterManager;
  //---------------------------

  //Load the "base" part
  loaderManager->load_cloud_part(path, lmin, lmax);
  Cloud* cloud_base = sceneManager->get_cloud_selected();
  Cloud* cloud_toAdd;

  //load and merge all other parts
  cout<<"Parameters : \n-> Nb lines per part : "<<nbLineSampling;
  cout<<"\n-> Space sampling : "<<spaceSampling<<endl;;
  while(lmax < size){
    cout<<"Loading part n°"<<nbPart;
    lmin = lmax;
    lmax = lmax + nbLinePart;
    nbPart++;

    //Load nth part
    loaderManager->load_cloud_part(path, lmin, lmax);
    cloud_toAdd = sceneManager->get_cloud_selected();

    //Merge the nth part with the base
    extractionManager->merging_addCloud(cloud_base, cloud_toAdd);

    //Supress nth added part
    sceneManager->remove_cloud(cloud_toAdd);

    //Space sampling the resulting cloud
    filterManager.spaceSampling(cloud_base, spaceSampling);
    cout<<" - "<<cloud_base->nb_point<<" points"<<endl;

    //Update final cloud
    sceneManager->update_cloud_glyphs(cloud_base);
  }
  */
  //---------------------------
}
void Pather::loading_treatment(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading("Cloud with processing loading");

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];

    //Load files
    loaderManager->load_cloud(path);

    //Save and remove
    Cloud* cloud = sceneManager->get_cloud_selected();
    saverManager->save_cloud(cloud, path);
    sceneManager->remove_cloud(cloud);
  }

  //---------------------------
}

//Other functions
void Pather::saving(){
  //---------------------------

  if(sceneManager->get_is_list_empty() == false){
    if(save_mode == "cloud"){
      Cloud* cloud = sceneManager->get_cloud_selected();
      this->saving_cloud(cloud);
    }
    else if(save_mode == "frame"){
      Cloud* cloud = sceneManager->get_cloud_selected();
      this->saving_cloud_frame(cloud);
    }
    else if(save_mode == "saved_frame"){
      this->saving_saved_frames();
    }
  }

  //---------------------------
}
void Pather::saving_cloud_frame(Cloud* cloud){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_directory();

  //Save current cloud
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    string path = path_saving + subset->name;

    if(subset != nullptr && path_saving != ""){
      saverManager->save_subset(subset, ".ply", path);
    }
  }

  //---------------------------
}
void Pather::saving_subset(Subset* subset){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_saving(subset->name);

  //Save current cloud
  if(subset != nullptr && path_saving != ""){
    saverManager->save_subset(subset, ".ply", path_saving);
  }

  //---------------------------
}
void Pather::saving_subset_range(int frame_b, int frame_e){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_directory();

  //Save current cloud
  if(cloud != nullptr && path_saving != ""){
    for(int i=frame_b; i<frame_e; i++){
      Subset* subset = sceneManager->get_subset(cloud, i);
      saverManager->save_subset(subset, ".ply", path_saving);
    }
  }

  //---------------------------
}
void Pather::saving_cloud(Cloud* cloud){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_saving(cloud->name);

  //Save current cloud
  if(cloud != nullptr && path_saving != ""){
    saverManager->save_cloud(cloud, path_saving);
  }

  //---------------------------
}
void Pather::saving_cloud_all(){
  //---------------------------

  //Select directory path
  string path_dir = this->zenity_directory();

  //Save all scene clouds
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    string pathFile = path_dir + "/" + cloud->name + ".pts";
    saverManager->save_cloud(cloud, pathFile);
  }

  //---------------------------
}
void Pather::saving_saved_frames(){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_directory();
  vector<string> file_path_vec = list_allPaths(path_saved_frame.c_str());
  vector<string> file_name_vec = list_allFiles(path_saved_frame.c_str());

  for(int i=0; i<file_path_vec.size(); i++){
    string file_in = file_path_vec[i];
    string file_out = path_saving + "/" + file_name_vec[i];

    std::ifstream src(file_in, std::ios::binary);
    std::ofstream dest(file_out, std::ios::binary);
    dest << src.rdbuf();
  }

  //---------------------------
}

//Specific functions
vector<string> Pather::get_directoryAllFilePath(string path){
  vector<string> allpath;
  //---------------------------

  for(const auto& entry : std::experimental::filesystem::directory_iterator(path)){
    allpath.push_back(entry.path());
  }

  //---------------------------
  return allpath;
}
string Pather::get_filePath(){
  //---------------------------

  string path_str = "";

  //Zenity window
  string zenity = "zenity --file-selection 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    path_str = string(path_char);
  }

  //Check if there is a /n
  if(path_str.find('\n')){
    path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
  }

  //---------------------------
  return path_str;
}
string Pather::get_filePath(string path){
  string path_str = "";
  //---------------------------

  //Zenity window
  string zenity = "zenity --file-selection --filename=" + path + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    path_str = string(path_char);
  }
  else if(path_char == NULL){
    path_str = path;
  }

  //Check if there is a /n
  if(path_str.find('\n')){
    path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
  }

  //---------------------------
  return path_str;
}
void Pather::selectDirectory(string& folderPath){
  string zenity = "zenity --file-selection --directory --title=Directory --filename=" + folderPath + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  //---------------------------

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    //Change path dir
    folderPath = path_str;
  }

  //---------------------------
}

//Zenity stuff
vector<string> Pather::zenity_loading(string title){
  //---------------------------

  //Open zenity file manager
  string path_current_dir = get_absolutePath_build() + '/';
  string zenity = "zenity --file-selection --multiple --title=" + title + " --filename=" + path_current_dir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  vector<string> path_vec;

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        string path_file = path_str.substr(0, path_str.find('|'));

        if (path_file.find('\n')){
          path_file.erase(std::remove(path_file.begin(), path_file.end(), '\n'), path_file.end());
        }

        path_vec.push_back(path_file);

        //Supress retrieved path from path list
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }

    //Retrieve the last selected path
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_vec.push_back(path_str);
  }

  //---------------------------
  return path_vec;
}
string Pather::zenity_saving(string filename){
  //---------------------------

  string path_saving = "";
  string path = path_current_dir + filename +".pts";

  //Open Zenity window
  string zenity = "zenity --file-selection --save --title=Save --filename=" + path;
  FILE *file = popen(zenity.c_str(), "r");
  char path_buffer[1024];
  char* path_char = fgets(path_buffer, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    //Supress unwanted line break
    string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_saving = path_str;
  }

  //Set current directory
  path_current_dir = path_saving.substr(0, path_saving.find_last_of("/") + 1);

  //---------------------------
  return path_saving;
}
string Pather::zenity_directory(){
  string path_directory = "";
  //---------------------------

  //Retrieve dir path
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_current_dir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    //Remove unwanted break line
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_directory = path_str;
  }

  //Set current directory
  path_current_dir = path_directory;

  //---------------------------
  return path_directory;
}
void Pather::zenity_select_directory(string& path_dir){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_dir;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
    }

    path_dir = path_str + "/";
  }

  //---------------------------
}
