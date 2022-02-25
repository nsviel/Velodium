#include "Saving.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Load/Load_node.h"
#include "../../../Load/Processing/Saver.h"

#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Saving::Saving(Engine_node* node_engine){
  //---------------------------

  Load_node* node_load = node_engine->get_node_load();

  this->renderManager = node_engine->get_renderManager();
  this->saverManager = node_load->get_saveManager();

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->path_frame = path_dir + "frame/";
  this->path_image = path_dir + "image/";

  this->save_frame_max = 20;
  this->save_image_ID = 0;
  this->save_image_max = 20;

  //---------------------------
}
Saving::~Saving(){}

//Output: frame & Image saving
void Saving::save_image(){
  //---------------------------

  //Save image
  string path = path_image + "image_" + to_string(save_image_ID);
  renderManager->render_screenshot(path);
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
void Saving::save_image_path(){
  //---------------------------

  string path;
  zenity_directory("", path);

  this->path_image = path + "/";

  //---------------------------
}
void Saving::save_frame(Subset* subset){
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
}
