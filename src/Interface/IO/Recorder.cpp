#include "Recorder.h"

#include "../Node_interface.h"
#include "../File/Directory.h"
#include "../File/Zenity.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Renderer.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"

#include <chrono>


//Constructor / Destructor
Recorder::Recorder(Node_interface* node){
  //---------------------------

  Node_engine* node_engine = node->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->node_interface = node_engine->get_node_interface();
  this->configManager = node_engine->get_configManager();
  this->renderManager = node_engine->get_renderManager();
  this->saverManager = node_load->get_saverManager();
  this->sceneManager = node_engine->get_sceneManager();

  //---------------------------
  this->update_configuration();
}
Recorder::~Recorder(){}

//Main functions
void Recorder::update_configuration(){
  //---------------------------

  this->with_save_frame = configManager->parse_json_b("dynamic", "with_save_frame");
  this->with_save_image = configManager->parse_json_b("dynamic", "with_save_image");
  this->with_save_frame_raw = false;

  this->path_dir = configManager->parse_json_s("parameter", "path_data");
  this->path_frame = configManager->parse_json_s("dynamic", "path_save_frame");
  this->path_image = configManager->parse_json_s("dynamic", "path_save_image");
  this->save_frame_max = configManager->parse_json_i("dynamic", "nb_save_frame");
  this->save_image_max = configManager->parse_json_i("dynamic", "nb_save_image");
  this->save_image_ID = 0;
  this->save_frame_accu = 1;

  this->check_directories();

  //---------------------------
}
void Recorder::compute_online(Cloud* cloud, int ID_subset){
  //---------------------------

  //Save subset frame
  if(with_save_frame){
    this->save_frame(cloud, ID_subset);
  }

  //Save rendered image
  if(with_save_image){
    this->save_image();
  }

  //---------------------------
}
void Recorder::clean_directories(){
  //---------------------------

  //Clean directories
  dir_clean_file(path_image.c_str());
  dir_clean_file(path_frame.c_str());

  //---------------------------
}
void Recorder::check_directories(){
  //---------------------------

  //Clean directories
  dir_create_new(path_dir);
  dir_create_new(path_image);
  dir_create_new(path_frame);

  //---------------------------
}

// Image saving
void Recorder::save_image(){
  //---------------------------

  if(save_image_max == 1){
    this->save_image_unique();
  }else if(save_image_max > 1){
    this->save_image_multiple();
  }else{
    return;
  }

  //---------------------------
}
void Recorder::save_image_unique(){
  //---------------------------

  //Put screenshot flag on
  string path = path_image + "image";
  *renderManager->get_save_path() = path;
  *renderManager->get_is_screenshot() = true;

  //---------------------------
}
void Recorder::save_image_multiple(){
  //---------------------------

  //Put screenshot flag on
  string path = path_image + "image_" + to_string(save_image_ID);
  *renderManager->get_save_path() = path;
  *renderManager->get_is_screenshot() = true;
  save_image_ID++;

  //Keep only a certain number of image
  if(save_image_vec.size() < save_image_max){
    save_image_vec.push(path);
  }else{
    std::remove (save_image_vec.front().c_str());
    save_image_vec.pop();
    save_image_vec.push(path);
  }

  //---------------------------
}
void Recorder::save_image_path(){
  //---------------------------

  zenity_directory(path_image);

  //---------------------------
}

// Frame saving
void Recorder::save_frame(Cloud* cloud, int ID_subset){
  //---------------------------

  if(with_save_frame_raw){
    Subset* subset = sceneManager->get_subset_init_byID(cloud, ID_subset);
    this->save_frame_subset(subset);
  }else{
    if(save_frame_accu == 1){
      Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
      this->save_frame_subset(subset);
    }else{
      this->save_frame_set(cloud, ID_subset);
    }
  }

  //---------------------------
}
void Recorder::save_frame_subset(Subset* subset){
  auto t1 = std::chrono::high_resolution_clock::now();
  //---------------------------

  //Save frame
  saverManager->save_subset_silent(subset, "ply", path_frame);

  //Keep only a certain number of frame
  string path = path_frame + subset->name + ".ply";
  if(save_frame_vec.size() < save_frame_max){
    save_frame_vec.push(path);
  }else{
    std::remove (save_frame_vec.front().c_str());
    save_frame_vec.pop();
    save_frame_vec.push(path);
  }

  //---------------------------
  auto t2 = std::chrono::high_resolution_clock::now();
  this->time_save_frame = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}
void Recorder::save_frame_set(Cloud* cloud, int ID_subset){
  Subset* subset = *next(cloud->subset.begin(), ID_subset);
  auto t1 = std::chrono::high_resolution_clock::now();
  //---------------------------

  //Save frame
  saverManager->save_set_silent(cloud, ID_subset, path_frame, save_frame_accu);

  //Keep only a certain number of frame
  string path = path_frame + subset->name + ".ply";
  if(save_frame_vec.size() < save_frame_max){
    save_frame_vec.push(path);
  }else{
    std::remove (save_frame_vec.front().c_str());
    save_frame_vec.pop();
    save_frame_vec.push(path);
  }

  //---------------------------
  auto t2 = std::chrono::high_resolution_clock::now();
  this->time_save_frame = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

//Path selection
void Recorder::select_path_image(){
  //---------------------------

  string path = zenity_directory("Path image", path_image);
  if(path != ""){
    this->path_image = path + "/";
  }

  //---------------------------
}
void Recorder::select_path_frame(){
  //---------------------------

  string path = zenity_directory("Path frame", path_frame);
  if(path != ""){
    this->path_frame = path + "/";
  }

  //---------------------------
}
void Recorder::select_image_unlimited(bool value){
  static int old_value;
  //---------------------------

  if(value){
    old_value = save_image_max;
    this->save_image_max = 999999;
  }else{
    this->save_image_max = old_value;
  }

  //---------------------------
}
void Recorder::select_frame_unlimited(bool value){
  static int old_value;
  //---------------------------

  if(value){
    old_value = save_frame_max;
    this->save_frame_max = 999999;
  }else{
    this->save_frame_max = old_value;
  }

  //---------------------------
}
