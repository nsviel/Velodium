#include "OnlinePlayer.h"

#include "../SLAM/CT_ICP.h"

#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Engine/OpenGL/Camera.h"


//Constructor / Destructor
OnlinePlayer::OnlinePlayer(Camera* camManager){
  //---------------------------

  this->cameraManager = camManager;
  this->heatmapManager = new Heatmap();
  this->slamManager = new CT_ICP();

  this->with_slam = false;
  this->with_camera_top = false;
  this->with_camera_follow = true;
  this->with_camera_root = false;
  this->with_cylinder_cleaning = true;
  this->with_heatmap = false;

  //---------------------------
}
OnlinePlayer::~OnlinePlayer(){}

//Main function
void OnlinePlayer::make_thing(Subset* subset){
  //---------------------------

  //If camera follow up option activated
  if(with_camera_follow){
    this->camera_followUp(subset);
  }

  //If with slam option is activated
  if(with_slam){
    //slamManager->compute_slam_OnlinePlayer(cloud, i);
  }

  //---------------------------
}
void OnlinePlayer::camera_followUp(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  if(with_camera_top){
    cameraManager->input_projView(2);
  }

  /*
  Frame* frame = &subset->frame;
  Eigen::Vector3d trans_b = frame->trans_b;
  vec3 camPos = cameraManager->get_camPos();

  float x = camPos.x + trans_b(0) - camera_moved.x;
  float y = camPos.y + trans_b(1) - camera_moved.y;
  float z = camPos.z;

  vec3 camPos_new = vec3(x, y, z);

  camera_moved = vec3(trans_b(0), trans_b(1), 0);
  cameraManager->set_cameraPos(camPos_new);
  */

  //Camera position
  /*vec3 trans_rlt = frame->trans_rlt;
  vec3* camPos = cameraManager->get_camPosPtr();
  *camPos = *camPos + trans_rlt;*/

  //Camera position
  if(with_camera_root){
    vec3* camPos = cameraManager->get_camPosPtr();
    *camPos = subset->root;
  }

  //Camera orientation
  Transforms transformManager;
  vec3 rotat_rlt = frame->rotat_rlt;
  float* hAngle = cameraManager->get_angle_azimuth();
  *hAngle = *hAngle + rotat_rlt.z * M_PI / 180;

  //---------------------------
}
