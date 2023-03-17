#include "Pather.h"

#include "Loader.h"
#include "Saver.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Specific/Function/fct_transtypage.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Zenity.h"
#include "../../Specific/File/Info.h"
#include "../../Specific/File/Path.h"

#include <Eigen/Dense>
#include <set>
#include <string>
#include <filesystem>


//Constructor / destructor
Pather::Pather(){
  //---------------------------

  this->configManager = new Configuration();
  this->sceneManager = new Scene();
  this->loaderManager = new Loader();
  this->saverManager = new Saver();
  this->data = Data::get_instance();

  //---------------------------
  this->update_configuration();
}
Pather::Pather(Node_load* node){
  //---------------------------

  this->configManager = new Configuration();
  this->sceneManager = new Scene();
  this->loaderManager = node->get_loaderManager();
  this->saverManager = node->get_saverManager();
  this->data = Data::get_instance();

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
  this->path_saved_frame = configManager->parse_json_s("parameter", "path_data")  + "frame";

  //---------------------------
}

//Loading functions
void Pather::loading_cloud(){
  //---------------------------

  //select files
  vector<string> path_vec = zenity_file_vec("Cloud loading", path_current_dir);

  //Load 1 collection by cloud
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    loaderManager->load_collection(path);
  }

  //---------------------------
}
void Pather::loading_frame(){
  //---------------------------

  //select files
  vector<string> path_vec = zenity_file_vec("Frame loading", path_current_dir);

  //Load 1 collection with all frame
  if(path_vec.size() != 0){
    loaderManager->load_collection_byFrame(path_vec);
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

//Specific loader function
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
  Collection* cloud_base = data->get_selected_collection();
  Collection* cloud_toAdd;

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
    cloud_toAdd = data->get_selected_collection();

    //Merge the nth part with the base
    extractionManager->merging_addCloud(cloud_base, cloud_toAdd);

    //Supress nth added part
    sceneManager->remove_collection(cloud_toAdd);

    //Space sampling the resulting cloud
    filterManager.spaceSampling(cloud_base, spaceSampling);
    cout<<" - "<<cloud_base->nb_point<<" points"<<endl;

    //Update final cloud
    sceneManager->update_glyph(collection_base);
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
    loaderManager->load_collection(path);

    //Save and remove
    Collection* collection = data->get_selected_collection();
    saverManager->save_collection(collection, path);
    sceneManager->remove_collection(collection);
  }

  //---------------------------
}

//Onthefly stuff
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
Collection* Pather::loading_onthefly(string path){
  Collection* collection = nullptr;
  //---------------------------

  //select files
  vector<string> path_vec = list_all_path(path);

  //Load files
  if(path_vec.size() != 0){
    collection = loaderManager->load_cloud_onthefly(path_vec);
  }

  //---------------------------
  return collection;
}
Collection* Pather::loading_directory_frame(string path){
  Collection* collection = nullptr;
  //---------------------------

  //Get all frame path
  vector<string> path_vec = list_all_path(path);

  //Sort alphabetically and load
  if(path_vec.size() != 0){
    fct_sort_alpha_num_(path_vec);
    collection = loaderManager->load_collection_byFrame(path_vec);
  }

  //---------------------------
  return collection;
}

//Other functions
void Pather::saving_cloud_frame(Collection* collection){
  //---------------------------

  //Select saving path
  string path_saving = zenity_directory("Save frame", path_current_dir);

  //Save current cloud
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    string path = path_saving + cloud->name;

    if(cloud != nullptr && path_saving != ""){
      saverManager->save_subset(cloud, ".ply", path);
    }
  }

  //---------------------------
}
void Pather::saving_subset(Cloud* cloud){
  //---------------------------

  //Select saving path
  string path_saving = zenity_saving(path_current_dir, cloud->name);

  //Save current cloud
  if(cloud != nullptr && path_saving != ""){
    saverManager->save_subset(cloud, "ply", path_saving);
  }

  //---------------------------
}
void Pather::saving_subset_range(int frame_b, int frame_e){
  Collection* collection = data->get_selected_collection();
  //---------------------------

  //Select saving path
  string path_saving = zenity_directory("Save", path_current_dir);

  //Save current cloud
  if(collection != nullptr && path_saving != ""){
    for(int i=frame_b; i<frame_e; i++){
      Cloud* cloud = (Cloud*)collection->get_obj(i);
      saverManager->save_subset(cloud, ".ply", path_saving);
    }
  }

  //---------------------------
}
void Pather::saving_cloud(Collection* collection){
  //---------------------------

  //Select saving path
  string path_saving = zenity_saving(path_current_dir, collection->name);

  //Save current collection
  if(collection != nullptr && path_saving != ""){
    saverManager->save_collection(collection, path_saving);
  }

  //---------------------------
}
void Pather::saving_cloud_same(Collection* collection){
  //---------------------------

  //Save current collection
  if(collection != nullptr && collection->path_file_load != ""){
    saverManager->save_collection(collection, collection->path_file_load);
  }

  //---------------------------
}
void Pather::saving_cloud_all(){
  //---------------------------

  //Select directory path
  string path_dir = zenity_directory("Save clouds", path_current_dir);

  //Save all scene clouds
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    string pathFile = path_dir + "/" + collection->name + ".pts";
    saverManager->save_collection(collection, pathFile);
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
