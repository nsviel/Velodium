#include "Followup.h"
#include "Camera.h"

#include "../Node_engine.h"
#include "../Core/Configuration.h"

#include "../../Specific/Function/fct_math.h"
#include "../../Specific/Function/fct_transtypage.h"


//Constructor / Destructor
Followup::Followup(Node_engine* node){
  //---------------------------

  this->configManager = node->get_configManager();
  this->cameraManager = node->get_cameraManager();

  //---------------------------
  this->update_configuration();
}
Followup::~Followup(){}

//Main function
void Followup::update_configuration(){
  //---------------------------

  this->camera_moved_trans = vec3(0, 0, 0);
  this->camera_distFromPos = 5;
  this->camera_nb_pose = 5;
  this->camera_top_z = 20;
  this->camera_oblique_z = 5;

  this->with_camera_follow = configManager->parse_json_b("camera", "with_camera_follow");
  this->with_camera_absolute = configManager->parse_json_b("camera", "with_camera_absolute");
  this->with_camera_top = configManager->parse_json_b("camera", "with_camera_top");

  //---------------------------
}
void Followup::camera_followup(Collection* collection, int ID_object){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(ID_object);
  if(cloud == nullptr) return;
  //---------------------------

  vec3 empty(0.0);
  if(collection->nb_obj >= camera_nb_pose && with_camera_follow){
    //Camera payload
    vec3 E = camera_payload(collection, ID_object);

    //Camera pose
    if(E != empty){
      this->camera_position(cloud, E);

      //Camera orientation
      this->camera_orientation(cloud, E);
    }
  }

  //---------------------------
}
void Followup::camera_mode(string mode){
  //---------------------------

  if(mode == "top"){
    cameraManager->set_mode_angle(0);
  }
  else if(mode == "oblique"){
    cameraManager->set_mode_angle(1);
  }

  //---------------------------
}

//Subfunctions
vec3 Followup::camera_payload(Collection* collection, int ID_object){
  Cloud* cloud_m0 = (Cloud*)collection->get_obj_byID(ID_object);
  Cloud* cloud_m1 = (Cloud*)collection->get_obj_byID(ID_object - 1);
  //---------------------------

  //Primilarly check
  vec3 E = vec3(0, 0, 0);
  if(cloud_m0 == nullptr) return E;
  if(cloud_m1 == nullptr) return E;
  if(collection->nb_obj < camera_nb_pose) return E;

  //Check if no stationary
  vec3 pos_m0 = eigen_to_glm_vec3(cloud_m0->pose_T);
  vec3 pos_m1 = eigen_to_glm_vec3(cloud_m1->pose_T);
  float pos_dist = fct_distance(pos_m0, pos_m1);
  if(pos_dist < 0.1) return E;

  //Retrieve the mean of some previous pose
  for(int i=0; i<camera_nb_pose; i++){
    Cloud* cloud = (Cloud*)collection->get_obj_byID(ID_object - i);

    if(cloud != nullptr){
      Eigen::Vector3d pos = cloud->pose_T;
      for(int j=0; j<3; j++){
        E[j] += pos(j);
      }
    }
  }
  for(int j=0; j<3; j++){
    E[j] = E[j] / camera_nb_pose;
  }

  //---------------------------
  return E;
}
void Followup::camera_position(Cloud* cloud, vec3 E){
  //---------------------------

  //Camera pose
  vec3 pose = eigen_to_glm_vec3(cloud->pose_T);
  vec3 C = pose - camera_distFromPos * (pose - E);

  //Forced absolute camera pose
  if(with_camera_absolute){
    //Top view
    if(with_camera_top){
      vec3 camPos = vec3(pose.x, pose.y, pose.z + camera_top_z);
      cameraManager->set_cameraPos(camPos);
      cameraManager->set_mode_angle(0);
    }
    //Oblique view
    else{
      vec3 camPos = vec3(C.x, C.y, pose.z + camera_oblique_z);
      cameraManager->set_cameraPos(camPos);
      cameraManager->set_mode_angle(1);
    }
  }
  //Forced relative camera pose
  else{
    vec3 camPos = cameraManager->get_cam_P();

    float x = camPos.x + pose[0] - camera_moved_trans.x;
    float y = camPos.y + pose[1] - camera_moved_trans.y;
    float z = camPos.z + pose[2] - camera_moved_trans.z;

    vec3 camPos_new = vec3(x, y, z);

    this->camera_moved_trans = vec3(pose[0], pose[1], pose[2]);
    cameraManager->set_cameraPos(camPos_new);
  }

  //---------------------------
}
void Followup::camera_orientation(Cloud* cloud, vec3 E){
  //---------------------------

  //Forced camera angle
  if(with_camera_absolute){
    vec3 pose = eigen_to_glm_vec3(cloud->pose_T);
    vec3 C = pose - camera_distFromPos * (pose - E);
    vec3 D = pose - C;
    float angle = atan(D.y, D.x) - atan(0.0f, 1.0f);

    cameraManager->set_angle_azimuth(angle);
  }
  //Relative camera angle
  else{

  }

  //---------------------------
}
void Followup::camera_reset(){
  //---------------------------

  this->camera_moved_trans = vec3(0, 0, 0);

  //---------------------------
}
