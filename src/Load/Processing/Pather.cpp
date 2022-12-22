#include "Pather.h"

#include "Loader.h"
#include "Saver.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_math.h"
#include "../../Interface/File/Directory.h"
#include "../../Interface/File/Zenity.h"
#include "../../Interface/File/Info.h"
#include "../../Interface/File/Path.h"

#include <Eigen/Dense>
#include <set>
#include <string>
#include <filesystem>


//Constructor / destructor
Pather::Pather(Node_load* node_load){
  //---------------------------

  Node_engine* node_engine = node_load->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();
  this->saverManager = node_load->get_saverManager();

  //---------------------------
  this->update_configuration();
}
Pather::~Pather(){
  //---------------------------


  //---------------------------
}

void Pather::update_configuration(){
  //---------------------------

  this->spaceSampling = 0.08f;
  this->nbLineSampling = 1000000;
  this->path_current_dir = get_path_abs_build();
  this->open_mode = configManager->parse_json_s("parameter", "open_mode");
  this->save_mode = configManager->parse_json_s("parameter", "save_mode");
  this->path_saved_frame = configManager->parse_json_s("parameter", "path_data")  + "frame";

  //---------------------------
}
bool Pather::check_folder_format(string path, string format){
  bool all_ok = true;
  //---------------------------

  vector<string> path_vec = list_all_path(path);
  for(int i=0; i<path_vec.size(); i++){
    if(get_format_from_path(path_vec[i]) != format){
      all_ok = false;
    }
  }

  //---------------------------
  return all_ok;
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
  vector<string> path_vec = zenity_file_vec("Cloud loading", path_current_dir);

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
  vector<string> path_vec = zenity_file_vec("Frame loading", path_current_dir);

  //Load files
  if(path_vec.size() != 0){
    loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
}
void Pather::loading_onthefly(){
  //---------------------------

  //select files
  vector<string> path_vec = zenity_file_vec("Frame loading", path_current_dir);

  //Load files
  if(path_vec.size() != 0){
    loaderManager->load_cloud_onthefly(path_vec);
  }

  //---------------------------
}
bool Pather::loading_onthefly(string path){
  bool ok = false;
  //---------------------------

  //select files
  vector<string> path_vec = list_all_path(path);

  //Load files
  if(path_vec.size() != 0){
    ok = loaderManager->load_cloud_onthefly(path_vec);
  }

  //---------------------------
  return ok;
}

bool Pather::loading_directory_frame(string path){
  bool ok = false;
  //---------------------------

  //Get all frame path
  vector<string> path_vec = list_all_path(path);

  //Sort alphabetically and load
  if(path_vec.size() != 0){
    fct_sort_alpha_num_(path_vec);
    ok = loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
  return ok;
}
void Pather::loading_sampling(){
  //---------------------------

  //select files
  vector<string> path_vec = zenity_file_vec("Cloud with sampling loading", path_current_dir);

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    int size = get_file_nbPoint(path);
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
  int size = get_file_nbPoint(path);
  Filter filterManager;
  //---------------------------

  //Load the "base" part
  loaderManager->load_cloud_part(path, lmin, lmax);
  Cloud* cloud_base = sceneManager->get_selected_cloud();
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
    cloud_toAdd = sceneManager->get_selected_cloud();

    //Merge the nth part with the base
    extractionManager->merging_addCloud(cloud_base, cloud_toAdd);

    //Supress nth added part
    sceneManager->remove_cloud(cloud_toAdd);

    //Space sampling the resulting cloud
    filterManager.spaceSampling(cloud_base, spaceSampling);
    cout<<" - "<<cloud_base->nb_point<<" points"<<endl;

    //Update final cloud
    sceneManager->update_cloud_glyph(cloud_base);
  }
  */
  //---------------------------
}
void Pather::loading_treatment(){
  //---------------------------

  //select files
  vector<string> path_vec = zenity_file_vec("Cloud with processing loading", path_current_dir);

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];

    //Load files
    loaderManager->load_cloud(path);

    //Save and remove
    Cloud* cloud = sceneManager->get_selected_cloud();
    saverManager->save_cloud(cloud, path);
    sceneManager->remove_cloud(cloud);
  }

  //---------------------------
}

//Other functions
void Pather::saving(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(sceneManager->get_is_list_empty() == false){
    if(save_mode == "cloud"){
      this->saving_cloud(cloud);
    }
    else if(save_mode == "frame"){
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
  string path_saving = zenity_directory("Save frame", path_current_dir);

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
  string path_saving = zenity_saving(path_current_dir, subset->name);

  //Save current cloud
  if(subset != nullptr && path_saving != ""){
    saverManager->save_subset(subset, "ply", path_saving);
  }

  //---------------------------
}
void Pather::saving_subset_range(int frame_b, int frame_e){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Select saving path
  string path_saving = zenity_directory("Save", path_current_dir);

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
  string path_saving = zenity_saving(path_current_dir, cloud->name);

  //Save current cloud
  if(cloud != nullptr && path_saving != ""){
    saverManager->save_cloud(cloud, path_saving);
  }

  //---------------------------
}
void Pather::saving_cloud_same(Cloud* cloud){
  //---------------------------

  //Save current cloud
  if(cloud != nullptr && cloud->path != ""){
    saverManager->save_cloud(cloud, cloud->path);
  }

  //---------------------------
}
void Pather::saving_cloud_all(){
  //---------------------------

  //Select directory path
  string path_dir = zenity_directory("Save clouds", path_current_dir);

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
  string path_saving = zenity_directory("Save frame", path_current_dir);
  vector<string> file_path_vec = list_all_path(path_saved_frame);
  vector<string> file_name_vec = list_all_file(path_saved_frame);

  for(int i=0; i<file_path_vec.size(); i++){
    string file_in = file_path_vec[i];
    string file_out = path_saving + "/" + file_name_vec[i];

    std::ifstream src(file_in, std::ios::binary);
    std::ofstream dest(file_out, std::ios::binary);
    dest << src.rdbuf();
  }

  //---------------------------
}
