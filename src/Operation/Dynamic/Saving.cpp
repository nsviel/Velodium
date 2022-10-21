#include "Saving.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Camera/Renderer.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Saver.h"

#include "../../Specific/fct_system.h"
#include "../../Specific/fct_zenity.h"

#include <chrono>


//Constructor / Destructor
Saving::Saving(Node_operation* node){
  this->node_ope = node;
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->node_interface = node_engine->get_node_interface();
  this->configManager = node_engine->get_configManager();
  this->renderManager = node_engine->get_renderManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->saverManager = node_load->get_saveManager();

  //---------------------------
  this->update_configuration();
}
Saving::~Saving(){}

void Saving::update_configuration(){
  //---------------------------

  this->with_save_frame = configManager->parse_json_b("interface", "with_save_frame");
  this->with_save_image = configManager->parse_json_b("interface", "with_save_image");

  this->path_dir = configManager->parse_json_s("parameter", "path_data");
  this->path_frame = configManager->parse_json_s("interface", "path_save_frame");
  this->path_image = configManager->parse_json_s("interface", "path_save_image");
  this->save_frame_max = configManager->parse_json_i("interface", "nb_save_frame");
  this->save_image_max = configManager->parse_json_i("interface", "nb_save_image");
  this->save_image_ID = 0;

  //---------------------------
}
void Saving::compute_online(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  //Save subset frame
  if(with_save_frame){
    this->save_frame(subset);
  }

  //Save rendered image
  if(with_save_image){
    this->save_image();
  }

  //---------------------------
}
void Saving::clean_directories(){
  //---------------------------

  //Clean directories
  clean_directory_files(path_image.c_str());
  clean_directory_files(path_frame.c_str());

  //---------------------------
}
void Saving::check_directories(){
  //---------------------------

  //Clean directories
  create_new_dir(path_dir);
  create_new_dir(path_image);
  create_new_dir(path_frame);

  //---------------------------
}

//Output: frame & Image saving
void Saving::save_image(){
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
void Saving::save_image_unique(){
  //---------------------------

  //Put screenshot flag on
  string path = path_image + "image";
  *renderManager->get_save_path() = path;
  *renderManager->get_is_screenshot() = true;

  //---------------------------
  this->path_image_last = path;
}
void Saving::save_image_multiple(){
  //---------------------------

  //Save image
  //string path = path_image + "image_" + to_string(save_image_ID);
  //renderManager->render_screenshot(path);

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
  this->path_image_last = path;
}
void Saving::save_image_path(){
  //---------------------------

  string path;
  zenity_directory("", path);

  this->path_image = path + "/";

  //---------------------------
}
void Saving::save_frame(Subset* subset){
  Frame* frame = &subset->frame;
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
  frame->time_save_frame = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}
