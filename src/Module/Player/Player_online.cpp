#include "Player_online.h"

#include "../SLAM/CT_ICP.h"

#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Transformation/Filter.h"
#include "../../Engine/OpenGL/Camera.h"
#include "../../Engine/Configuration/Dimension.h"
#include "../../Engine/Engine.h"
#include "../../Specific/fct_maths.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../extern/stb_image/stb_image_write.h"


//Constructor / Destructor
Player_online::Player_online(Engine* engineManager){
  //---------------------------

  this->cameraManager = engineManager->get_CameraManager();
  this->filterManager = engineManager->get_filterManager();
  this->dimManager = engineManager->get_dimManager();
  this->heatmapManager = new Heatmap();
  this->cticpManager = new CT_ICP();

  this->HM_height_range = vec2(-2.25, 1.75);
  this->camera_moved_trans = vec2(0, 0);
  this->camera_moved_rotat = 0;
  this->camera_distFromLidarPos = 5;

  this->with_camera_top = false;
  this->with_camera_follow = true;
  this->with_camera_root = false;

  this->with_online = false;
  this->with_slam = true;
  this->with_cylinder_cleaning = true;
  this->with_heatmap = true;
  this->with_heatmap_rltHeight = true;
  this->with_save_image = false;

  //---------------------------
}
Player_online::~Player_online(){}

//Main function
void Player_online::compute_onlineOpe(Cloud* cloud, int i){
  Subset* subset = &cloud->subset[i];
  //---------------------------

  if(with_online){
    //Ortho top view option
    if(with_camera_top){
      cameraManager->input_projView(2);
    }

    //If with slam option is activated
    if(with_slam){
      //Don't forget to operate the first subset
      if(i == 1){
        cticpManager->compute_slam_online(cloud, 0);
      }

      //Make slam on the current subset
      cticpManager->compute_slam_online(cloud, i);
    }

    //If camera follow up option activated
    if(with_camera_follow){
      this->camera_followUp(cloud, i);
    }

    //If cylinder cleaning option
    if(with_cylinder_cleaning){
      //Don't forget to operate the first subset
      if(i == 1){
        Subset* subset = &cloud->subset[0];
        filterManager->filter_subset_cylinder(subset);
      }

      //Make cleaning on the current subset
      filterManager->filter_subset_cylinder(subset);
    }

    //If heatmap option
    if(with_heatmap){
      if(with_heatmap_rltHeight){
        vec2* HT_range = heatmapManager->get_height_range();
        HT_range->x = subset->frame.trans_abs.z + HM_height_range.x;
        HT_range->y = subset->frame.trans_abs.z + HM_height_range.y;
      }

      //Don't forget to operate the first subset
      if(i == 1){
        Subset* subset = &cloud->subset[0];
        Subset* subset_buffer = &cloud->subset_buffer[0];
        heatmapManager->set_Heatmap(subset, subset_buffer, with_heatmap);
      }

      //Make heatmap on the current subset
      Subset* subset_buffer = &cloud->subset_buffer[i];
      heatmapManager->set_Heatmap(subset, subset_buffer, with_heatmap);
    }

    if(with_save_image){
      this->save_image();
    }
  }

  //---------------------------
}

void Player_online::camera_followUp(Cloud* cloud, int i){
  //---------------------------

  Frame* frame = &cloud->subset[i].frame;

  if(frame->ID >= 2){
    Frame* frame_m1 = &cloud->subset[i-1].frame;

    vec3 pos = frame->trans_abs;
    vec3 pos_m1 = frame_m1->trans_abs;

    vec3 E = pos - pos_m1;
    vec3 C = pos_m1 - camera_distFromLidarPos / fct_distance_origin(E) * E;

    if(E != vec3(0,0,0)){
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
void Player_online::save_image(){
  //---------------------------

  GLFWwindow* window = dimManager->get_window();
  string path = "/home/aither/Desktop/truc";

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  GLsizei nrChannels = 3;
  GLsizei stride = nrChannels * width;
  stride += (stride % 4) ? (4 - stride % 4) : 0;
  GLsizei bufferSize = stride * height;
  std::vector<char> buffer(bufferSize);
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
  stbi_flip_vertically_on_write(true);
  stbi_write_png(path.c_str(), width, height, nrChannels, buffer.data(), stride);

  //---------------------------
}
