#include "Followup.h"

#include "../Player_node.h"
#include "../../Module_node.h"

#include "../../../Engine/OpenGL/Camera/Camera.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
Followup::Followup(Player_node* node){
  //---------------------------

  Module_node* node_module = node->get_node_module();
  Engine_node* node_engine = node_module->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();

  this->sceneManager = node_engine->get_sceneManager();
  this->cameraManager = node_engine->get_cameraManager();

  this->camera_moved_trans = vec2(0, 0);
  this->camera_moved_rotat = 0;
  this->camera_distFromPos = 5;
  this->camera_nb_pose = 5;

  this->with_camera_follow = configManager->parse_json_b("module", "with_camera_follow");
  this->with_camera_absolute = true;
  this->with_camera_top = false;
  this->with_camera_root = false;

  //---------------------------
}
Followup::~Followup(){}

//Main function
void Followup::camera_followUp(Cloud* cloud, int ID_subset){
  //---------------------------

  //Follow up camera
  if(with_camera_follow){
    this->camera_displacment(cloud, ID_subset);
  }

  //Ortho top view option
  if(with_camera_top){
    cameraManager->input_projView(2);
  }else{
    cameraManager->input_projView(0);
  }

  //---------------------------
}

//Subfunctions
void Followup::camera_displacment(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  Frame* frame_m0 = &subset->frame;

  if(frame_m0->ID >= camera_nb_pose){
    //Camera payload
    vec3 E = camera_payload(cloud, ID_subset);

    //Camera pose
    this->camera_position(subset, E);

    //Camera orientation
    this->camera_orientation(subset, E);
  }

  //---------------------------
}
vec3 Followup::camera_payload(Cloud* cloud, int ID_subset){
  vec3 E = vec3(0, 0, 0);
  //---------------------------

  Frame* frame_m0 = sceneManager->get_frame_byID(cloud, ID_subset);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID_subset-1);
  vec3 pos_m0 = frame_m0->trans_abs;
  vec3 pos_m1 = frame_m1->trans_abs;
  float pos_dist = fct_distance(pos_m0, pos_m1);

  //If the displacment is enough
  if(pos_dist > 0.1){
    //Sum all pose values
    for(int i=0; i<camera_nb_pose; i++){
      Frame* frame = sceneManager->get_frame_byID(cloud, ID_subset-i);
      vec3& pos = frame->trans_abs;

      for(int j=0; j<3; j++){
        E[j] += pos[j];
      }
    }

    //Get mean
    for(int j=0; j<3; j++){
      E[j] = E[j] / camera_nb_pose;
    }
  }

  //---------------------------
  return E;
}
void Followup::camera_position(Subset* subset, vec3 E){
  Frame* frame = &subset->frame;
  //---------------------------

  //Camera pose
  vec3 C = frame->trans_abs - camera_distFromPos * (E / fct_distance_origin(E));

  if(with_camera_absolute){
    vec3 camPos = cameraManager->get_camPos();
    vec3 camPos_new = vec3(C.x, C.y, camPos.z);
    cameraManager->set_cameraPos(camPos_new);
  }else{
    Eigen::Vector3f trans_b = frame->trans_b;
    vec3 camPos = cameraManager->get_camPos();

    float x = camPos.x + trans_b(0) - camera_moved_trans.x;
    float y = camPos.y + trans_b(1) - camera_moved_trans.y;
    float z = camPos.z;

    vec3 camPos_new = vec3(x, y, z);

    this->camera_moved_trans = vec3(trans_b(0), trans_b(1), 0);
    cameraManager->set_cameraPos(camPos_new);
  }

  //---------------------------
}
void Followup::camera_orientation(Subset* subset, vec3 E){
  Frame* frame = &subset->frame;
  //---------------------------

  if(with_camera_absolute){
    float cam_angle = atan(E.y, E.x) - atan(0.0f, 1.0f);
    cameraManager->set_angle_azimuth(cam_angle);
  }

  //---------------------------
}
void Followup::camera_reset(){
  //---------------------------

  this->camera_moved_trans = vec3(0, 0, 0);

  //---------------------------
}
