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
  this->camera_distPos = 5;

  this->with_camera_absolute = false;
  this->with_camera_top = false;
  this->with_camera_follow = configManager->parse_json_b("module", "with_camera_follow");
  this->with_camera_root = false;

  //---------------------------
}
Followup::~Followup(){}

//Camera funtions
void Followup::camera_followUp(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  Frame* frame = &subset->frame;

  if(frame->ID >= 4){
    Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID_subset-1);
    Frame* frame_m2 = sceneManager->get_frame_byID(cloud, ID_subset-2);
    Frame* frame_m3 = sceneManager->get_frame_byID(cloud, ID_subset-3);

    vec3 pos_m0 = frame->trans_abs;
    vec3 pos_m1 = frame_m1->trans_abs;
    vec3 pos_m2 = frame_m2->trans_abs;
    vec3 pos_m3 = frame_m3->trans_abs;
    vec3 pos_diff = pos_m0 - pos_m1;

    //If the displacment is enough
    if(pos_diff.x > 0.1 || pos_diff.y > 0.1){
      vec3 E = vec3(0,0,0);
      for(int i=0; i<3; i++){
        E[i] += pos_m0[i];
        E[i] += pos_m1[i];
        E[i] += pos_m2[i];
        E[i] += pos_m3[i];

        E[i] = E[i] / 4;
      }

      E = pos_m0 - E;
      vec3 C = pos_m1 - camera_distPos * (E / fct_distance_origin(E));

      //Camera pose
      this->camera_position(subset, C);

      //Camera orientation
      this->camera_orientation(E);
    }
  }

  //---------------------------
}
void Followup::camera_position(Subset* subset, vec3 C){
  Frame* frame = &subset->frame;
  //---------------------------

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
void Followup::camera_orientation(vec3 E){
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
