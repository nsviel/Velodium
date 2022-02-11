#include "Player_online.h"

#include "../Module_node.h"
#include "../SLAM/Slam.h"
#include "../Obstacle/Interfacing.h"

#include "../../Operation/Operation_node.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/OpenGL/Camera.h"
#include "../../Engine/OpenGL/Renderer.h"
#include "../../Engine/OpenGL/Dimension.h"

#include "../../Engine/Configuration/Configuration_node.h"
#include "../../Engine/Configuration/config_module.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene.h"

#include "../../Load/Pather.h"

#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Player_online::Player_online(Module_node* node_module){
  //---------------------------

  Operation_node* node_ope = node_module->get_node_ope();
  Engine_node* node_engine = node_module->get_node_engine();
  Configuration_node* node_config = node_engine->get_node_config();

  this->filterManager = node_ope->get_filterManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->dimManager = node_engine->get_dimManager();
  this->slamManager = node_module->get_slamManager();
  this->configManager = node_config->get_conf_modManager();
  this->ioManager = node_module->get_ioManager();
  this->sceneManager = new Scene();

  this->visibility_range = 15;

  //---------------------------
  this->update_configuration();
}
Player_online::~Player_online(){}

//Main function
void Player_online::compute_onlineOpe(Cloud* cloud, int ID_subset){
  //This function is called each time a new subset arrives
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  //Control subset visibilities
  this->set_cloud_visibility(cloud, ID_subset);

  //Compute stuff for selected subset
  if(with_online){
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

    //Colorization options
    if(with_heatmap){
      this->color_heatmap(cloud, ID_subset);
    }
    else if(with_unicolor){
      this->color_unicolor(subset, cloud->unicolor);
    }

    //Save subset frame
    if(with_save_frame){
      Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
      ioManager->save_frame(subset);
    }

    //Save rendered image
    if(with_save_image){
      tic();
      ioManager->save_image();
      this->time_image = toc();
    }

    //Provide info about computation
    this->compute_statistics(subset);
  }

  //---------------------------
}
void Player_online::compute_statistics(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Consol result
  string stats = subset->name + " [" + to_string(subset->ID) + "]: ";
  if(with_save_image){
    stats += "[SLAM- " + to_string((int)frame->time_slam) + " ms] ";
  }
  if(with_save_image){
    stats += "[IMAG- " + to_string((int)time_image) + " ms] ";
  }
  console.AddLog("#", stats);

  //---------------------------
}
void Player_online::update_configuration(){
  //---------------------------

  this->HM_height_range = vec2(-2.5, 1.75);
  this->camera_moved_trans = vec2(0, 0);
  this->camera_moved_rotat = 0;
  this->camera_distPos = 5;

  this->with_camera_top = false;
  this->with_camera_follow = configManager->parse_json_b("online", "with_camera_follow");
  this->with_camera_root = false;

  this->with_heatmap = configManager->parse_json_b("online", "with_heatmap");
  this->with_heatmap_rltHeight = true;
  this->with_unicolor = !with_heatmap;

  this->with_online = true;
  this->with_save_frame = true;
  this->with_save_image = true;
  this->with_slam = true;// configManager->parse_json_b("online", "with_slam");
  this->with_cylinder_cleaning = false;//configManager->parse_json_b("online", "with_cylinder_cleaning");
  this->with_remove_lastSubset = true;//configManager->parse_json_b("online", "with_remove_lastSubset");

  //---------------------------
}

//Camera funtions
void Player_online::camera_followUp(Cloud* cloud, int ID_subset){
  //---------------------------

  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  Frame* frame = &subset->frame;

  if(frame->ID >= 4){
    Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID_subset-1);
    Frame* frame_m2 = sceneManager->get_frame_byID(cloud, ID_subset-2);
    Frame* frame_m3 = sceneManager->get_frame_byID(cloud, ID_subset-3);

    vec3 pos_m0 = frame->trans_abs;
    vec3 pos_m1 = frame_m1->trans_abs;
    vec3 pos_m2 = frame_m2->trans_abs;
    vec3 pos_m3 = frame_m3->trans_abs;

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

  //this->camera_position(subset);
  //this->camera_orientation(subset);

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

//Other functions
void Player_online::set_cloud_visibility(Cloud* cloud, int& ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  //Unset alll other subset visibility
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset_loop = sceneManager->get_subset(cloud, i);
    subset_loop->visibility = false;
  }

  //Set visibility just for wanted subsets
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset_loop = sceneManager->get_subset(cloud, i);

    if(subset_loop->ID > ID_subset - visibility_range && subset_loop->ID <= ID_subset){
      subset_loop->visibility = true;
    }
  }

  //---------------------------
}
void Player_online::color_heatmap(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  if(with_heatmap_rltHeight){
    vec2* HT_range = heatmapManager->get_height_range();
    HT_range->x = subset->frame.trans_abs.z + HM_height_range.x;
    HT_range->y = subset->frame.trans_abs.z + HM_height_range.y;
  }

  //Make heatmap on the current subset
  Subset* subset_buffer = sceneManager->get_subset_buffer_byID(cloud, ID_subset);
  heatmapManager->set_Heatmap(subset, subset_buffer, with_heatmap);

  //---------------------------
}
void Player_online::color_unicolor(Subset* subset, vec4 color){
  vector<vec4>& RGB = subset->RGB;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = color;
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}
