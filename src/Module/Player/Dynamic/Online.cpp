#include "Online.h"

#include "../../Module_node.h"
#include "../../SLAM/Slam.h"
#include "../../Interface/Interface_node.h"
#include "../../Interface/Local/Saving.h"
#include "../../Interface/LiDAR/Capture.h"

#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Color/Color.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Operation/Transformation/Filter.h"

#include "../../../Engine/OpenGL/Camera/Camera.h"
#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Engine/OpenGL/Dimension.h"

#include "../../../Engine/Engine.h"
#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"

#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Online::Online(Module_node* node_module){
  //---------------------------

  Operation_node* node_ope = node_module->get_node_ope();
  Engine_node* node_engine = node_module->get_node_engine();

  this->filterManager = node_ope->get_filterManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->dimManager = node_engine->get_dimManager();
  this->slamManager = node_module->get_slamManager();
  this->configManager = node_engine->get_configManager();
  this->node_interface = node_module->get_node_interface();
  this->sceneManager = node_engine->get_sceneManager();
  this->colorManager = node_ope->get_colorManager();

  this->visibility_range = 15;

  //---------------------------
  this->update_configuration();
}
Online::~Online(){}

//Main function
void Online::update_configuration(){
  //---------------------------

  this->camera_moved_trans = vec2(0, 0);
  this->camera_moved_rotat = 0;
  this->camera_distPos = 5;
  this->nb_subset_max = 50;

  this->with_camera_top = false;
  this->with_camera_follow = configManager->parse_json_b("module", "with_camera_follow");
  this->with_camera_root = false;
  this->with_justOneFrame = false;
  this->with_subset_specific_color = false;
  this->with_save_frame = configManager->parse_json_b("interface", "with_save_frame");
  this->with_save_image = configManager->parse_json_b("interface", "with_save_image");
  this->with_slam = configManager->parse_json_b("module", "with_slam");
  this->with_cylinder_cleaning = configManager->parse_json_b("module", "with_cylinder_cleaning");
  this->with_remove_lastSubset = configManager->parse_json_b("interface", "with_remove_lastSubset");

  //---------------------------
}
void Online::compute_onlineOpe(Cloud* cloud, int ID_subset){
  //This function is called each time a new subset arrives
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  cloud->subset_selected = subset;
  //---------------------------

  //Control subset visibilities
  this->set_cloud_visibility(cloud, ID_subset);

  //Ortho top view option
  if(with_camera_top){
    cameraManager->input_projView(2);
  }

  //Make slam on the current subset
  if(with_slam){
    slamManager->compute_slam_online(cloud, ID_subset);
  }

  //If camera follow up option activated
  if(with_camera_follow){
    this->camera_followUp(cloud, ID_subset);
  }

  //Make cleaning on the current subset
  if(with_cylinder_cleaning){
    filterManager->filter_subset_cylinder(subset);
  }

  //Colorization
  if(with_subset_specific_color){
    colorManager->make_colorization_specific(subset);
    if(ID_subset>2){
      Subset* subset_m1 = sceneManager->get_subset_byID(cloud, ID_subset-1);
      colorManager->make_colorization(subset_m1);
    }
  }else{
    colorManager->make_colorization(subset);
  }

  //Save subset frame
  if(with_save_frame){
    Saving* saveManager = node_interface->get_saveManager();
    saveManager->save_frame(subset);
  }

  //Save rendered image
  if(with_save_image){
    Saving* saveManager = node_interface->get_saveManager();
    saveManager->save_image();
  }

  //Regulate the number of cloud frame
  this->cloud_size_controler(cloud);

  //Provide info about computation
  this->compute_displayStats(subset);

  //---------------------------
}
void Online::compute_displayStats(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Consol result
  string stats = subset->name + ": ";
  if(with_slam && frame->is_slamed){
    stats += "[SLAM- " + to_string((int)frame->time_slam) + " ms] ";
  }
  if(with_save_frame){
    stats += "[Frame- " + to_string((int)frame->time_save_frame) + " ms] ";
  }
  console.AddLog("#", stats);

  //---------------------------
}

//Camera funtions
void Online::camera_followUp(Cloud* cloud, int ID_subset){
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
      vec3 camPos = cameraManager->get_camPos();
      vec3 camPos_new = vec3(C.x, C.y, camPos.z);
      cameraManager->set_cameraPos(camPos_new);

      //Camera orientation
      float cam_angle = atan(E.y, E.x) - atan(0.0f, 1.0f);
      cameraManager->set_angle_azimuth(cam_angle);
    }
  }

  //this->camera_position(subset);
  //this->camera_orientation(subset);

  //---------------------------
}
void Online::camera_position(Subset* subset){
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
    Eigen::Vector3f trans_b = frame->trans_b;
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
void Online::camera_orientation(Subset* subset){
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

//Other functions
void Online::cloud_size_controler(Cloud* cloud){
  //---------------------------

  //If option, remove all other subset
  if(with_justOneFrame){
    sceneManager->remove_subset_last(cloud);
  }
  //Remove old frame if option is activated
  else{
    Capture* captureManager = node_interface->get_captureManager();
    bool is_capturing = captureManager->get_is_capture_watcher();
    if(cloud->subset.size() > nb_subset_max && is_capturing){
      sceneManager->remove_subset_last(cloud);
    }
  }

  //---------------------------
}
void Online::set_cloud_visibility(Cloud* cloud, int& ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  //Set visibility just for wanted subsets
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID > ID_subset - visibility_range && subset->ID <= ID_subset){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }
  }

  //---------------------------
}
