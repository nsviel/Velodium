#include "Player_online.h"

#include "../SLAM/CT_ICP.h"

#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Transformation/Filter.h"
#include "../../Engine/OpenGL/Camera.h"
#include "../../Engine/Engine.h"


//Constructor / Destructor
Player_online::Player_online(Engine* engineManager){
  //---------------------------

  this->cameraManager = engineManager->get_CameraManager();
  this->heatmapManager = new Heatmap();
  this->cticpManager = new CT_ICP();
  this->filterManager = new Filter();

  this->camera_moved_trans = vec2(0, 0);
  this->camera_moved_rotat = 0;

  this->with_camera_top = false;
  this->with_camera_follow = true;
  this->with_camera_root = false;

  this->with_slam = true;
  this->with_cylinder_cleaning = false;
  this->with_heatmap = false;

  //---------------------------
}
Player_online::~Player_online(){}

//Main function
void Player_online::compute_onlineOpe(Cloud* cloud, int i){
  Subset* subset = &cloud->subset[i];
  //---------------------------

  //Ortho top view option
  if(with_camera_top){
    cameraManager->input_projView(2);
  }

  //If camera follow up option activated
  if(with_camera_follow){
    this->camera_followUp(subset);
  }

  //If with slam option is activated
  if(with_slam){
    //Don't forget to make slam on the first subset
    if(i == 1){
      cticpManager->compute_slam_online(cloud, 0);
    }

    //Make slam on the current subset
    cticpManager->compute_slam_online(cloud, i);
  }

  //If cylinder cleaning option
  if(with_cylinder_cleaning){
    filterManager->filter_subset_cylinder(subset);
  }

  //---------------------------
}
void Player_online::reset(){
  //---------------------------

  //cticpManager->reset();

  //---------------------------
}

void Player_online::camera_followUp(Subset* subset){
  //---------------------------

  this->camera_position(subset);
  this->camera_orientation(subset);

  //---------------------------
}
void Player_online::camera_position(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  /*
  //Camera relative movement
  vec3 trans_rlt = frame->trans_rlt;
  vec3* camPos = cameraManager->get_camPosPtr();
  *camPos = *camPos + trans_rlt;
  */

  //Camera attached to subset root
  if(with_camera_root){
    vec3* camPos = cameraManager->get_camPosPtr();
    *camPos = subset->root;
  }
  //Camera follow absolute movement
  else{
    Frame* frame = &subset->frame;
    Eigen::Vector3d trans_b = frame->trans_b;
    vec3 camPos = cameraManager->get_camPos();

    float x = camPos.x + trans_b(0) - camera_moved_trans.x;
    float y = camPos.y + trans_b(1) - camera_moved_trans.y;
    float z = camPos.z;

    vec3 camPos_new = vec3(x, y, z);

    camera_moved_trans = vec3(trans_b(0), trans_b(1), 0);
    cameraManager->set_cameraPos(camPos_new);
  }

  //---------------------------
}
void Player_online::camera_orientation(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  /*
  Transforms transformManager;
  vec3 rotat_rlt = frame->rotat_rlt;
  float* hAngle = cameraManager->get_angle_azimuth();
  *hAngle = *hAngle + rotat_rlt.z * M_PI / 180;
  */

  Transforms transformManager;
  vec3 rotat_abs = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
  float hAngle = *cameraManager->get_angle_azimuth();

  float rotat_abs_rad = rotat_abs.z * M_PI / 180;
  float hAngle_new = hAngle + rotat_abs_rad - camera_moved_rotat;

  camera_moved_rotat = rotat_abs_rad;
  cameraManager->set_angle_azimuth(hAngle_new);

  //---------------------------
}
