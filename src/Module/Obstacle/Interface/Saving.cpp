#include "Saving.h"

#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Load/Saver.h"
#include "../../../Load/Pather.h"

#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Saving::Saving(Renderer* renderer){
  //---------------------------

  this->renderManager = renderer;
  this->saverManager = new Saver();
  this->pathManager = new Pather();

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
  Pather pathManager;
  pathManager.selectDirectory(path);

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
