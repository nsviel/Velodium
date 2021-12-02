#include "GUI_slam.h"

#include "../CT_ICP.h"
#include "../CT_ICP/SLAM_optim_ceres.h"
#include "../CT_ICP/SLAM_optim_gn.h"

#include "../../../Engine/Data/Database.h"
#include "../../../Engine/Scene.h"
#include "../../../Specific/fct_transtypage.h"

extern struct Database database;


//Constructor / Destructor
GUI_slam::GUI_slam(){
  //---------------------------

  this->cticpManager = new CT_ICP();
  this->ceresManager = cticpManager->get_SLAM_optim_ceres();
  this->gnManager = cticpManager->get_SLAM_optim_gn();

  //---------------------------
}
GUI_slam::~GUI_slam(){}

//Main function
void GUI_slam::design_SLAM(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  if(ImGui::Button("Compute", ImVec2(75,0))){
    cticpManager->compute_slam();

    Scene sceneManager;
    sceneManager.update_cloud_location(cloud);
  }

  this->parameters();
  this->statistics();

  //---------------------------
}

void GUI_slam::parameters(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  float* sampling_size = cticpManager->get_sampling_size();
  ImGui::InputFloat("Subsample grid size", sampling_size, 0.1f, 1.0f, "%.3f");

  static int iter_max = 1;
  if(ImGui::SliderInt("Number iter", &iter_max, 1, 20)){
    ceresManager->set_iter_max(iter_max);
    gnManager->set_iter_max(iter_max);
  }

  static int frame_max = 10;
  int max = 1;
  if(cloud != nullptr){
    max = cloud->nb_subset;
  }
  if(ImGui::SliderInt("Number frame", &frame_max, 1, max)){
    cticpManager->set_frame_max(frame_max);
  }

  //---------------------------
}
void GUI_slam::statistics(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  Eigen::Vector3d trans_b = Eigen::Vector3d::Zero();
  Eigen::Vector3d trans_e = Eigen::Vector3d::Zero();
  vec3 rotat_b(0,0,0);
  vec3 rotat_e(0,0,0);

  if(cloud != nullptr){
    Frame* frame = &cloud->subset[cloud->subset_selected].frame;

    trans_b = frame->trans_b;
    trans_e = frame->trans_e;

    mat4 mat_b = eigen_to_glm_mat4(frame->rotat_b);
    mat4 mat_e = eigen_to_glm_mat4(frame->rotat_e);

    rotat_b = compute_anglesFromTransformationMatrix(mat_b);
    rotat_e = compute_anglesFromTransformationMatrix(mat_e);
  }

  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "Tb [m]: %.3f %.3f %.3f", trans_b(0), trans_b(1), trans_b(2));
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "Te [m]: %.3f %.3f %.3f", trans_e(0), trans_e(1), trans_e(2));
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "Rb [°]: %.3f %.3f %.3f", rotat_b.x, rotat_b.y, rotat_b.z);
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "Re [°]: %.3f %.3f %.3f", rotat_e.x, rotat_e.y, rotat_e.z);

  //---------------------------
}

vec3 GUI_slam::compute_anglesFromTransformationMatrix(const mat4& mat){
  vec3 angles;
  //---------------------------

  float ax = atan2(mat[2][1], mat[2][2]);
  float ay = atan2(-mat[2][0], sqrt( pow(mat[2][1], 2) + pow(mat[2][2], 2) ) );
  float az = atan2(mat[1][0], mat[0][0]);

  ax = (ax * 180) / M_PI;
  ay = (ay * 180) / M_PI;
  az = (az * 180) / M_PI;

  angles = vec3(ax, ay, az);

  //---------------------------
  return angles;
}
