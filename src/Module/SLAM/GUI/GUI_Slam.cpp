#include "GUI_Slam.h"

#include "../CT_ICP.h"
#include "../CT_ICP/SLAM_optim_ceres.h"
#include "../CT_ICP/SLAM_optim_gn.h"

#include "../../../Engine/Data/Database.h"
#include "../../../Engine/Scene.h"
#include "../../../Specific/fct_transtypage.h"

extern struct Database database;


//Constructor / Destructor
GUI_Slam::GUI_Slam(){
  //---------------------------

  this->cticpManager = new CT_ICP();
  this->ceresManager = cticpManager->get_SLAM_optim_ceres();
  this->gnManager = cticpManager->get_SLAM_optim_gn();

  this->item_width = 100;

  //---------------------------
}
GUI_Slam::~GUI_Slam(){}

//Main function
void GUI_Slam::design_SLAM(){
  //---------------------------

  this->compute();
  this->parameters();
  this->statistics();

  //---------------------------
}

void GUI_Slam::compute(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Compute", ImVec2(item_width,0))){
    Scene sceneManager;

    if(cloud != nullptr){
      sceneManager.update_cloud_reset(cloud);
      cticpManager->compute_slam();
      sceneManager.update_cloud_location(cloud);
    }
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Slam::parameters(){
  if(ImGui::CollapsingHeader("Parameters")){
    Cloud* cloud = database.cloud_selected;
    //---------------------------

    //
    bool* slam_verbose = cticpManager->get_verbose();
    ImGui::Checkbox("Verbose", slam_verbose);

    //Subsampling voxel width
    float* sampling_width = cticpManager->get_sampling_width();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Subsample grid size", sampling_width, 0.1f, 1.0f, "%.3f");

    //Width of the local map voxel
    float* map_width = cticpManager->get_mapVoxel_width();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Map voxel size", map_width, 0.1f, 1.0f, "%.3f");

    //Number of optimization iterations
    static int iter_max = 5;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number iter", &iter_max, 1, 20)){
      ceresManager->set_iter_max(iter_max);
      gnManager->set_iter_max(iter_max);
    }

    //Number of frame computed
    if(cloud != nullptr){
      static int frame_max = cloud->nb_subset;
      ImGui::SetNextItemWidth(item_width);
      if(ImGui::SliderInt("Number frame", &frame_max, 1, cloud->nb_subset)){
        cticpManager->set_frame_all(false);
        cticpManager->set_frame_max(frame_max);
      }
    }else{
      static int frame_max = 0;
      ImGui::SetNextItemWidth(item_width);
      ImGui::SliderInt("Number frame", &frame_max, 0, 0);
    }

    //Number of thread for the normal computation
    static int nb_thread = 8;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number thread", &nb_thread, 0, 20)){
      SLAM_normal* normalManager = cticpManager->get_SLAM_normal();
      normalManager->set_nb_thread(nb_thread);
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Slam::statistics(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  Eigen::Vector3d trans_b = Eigen::Vector3d::Zero();
  Eigen::Vector3d trans_e = Eigen::Vector3d::Zero();
  vec3 rotat_b(0,0,0);
  vec3 rotat_e(0,0,0);
  float time_slam = 0;

  if(cloud != nullptr){
    Frame* frame = &cloud->subset[cloud->subset_selected].frame;

    trans_b = frame->trans_b;
    trans_e = frame->trans_e;

    mat4 mat_b = eigen_to_glm_mat4(frame->rotat_b);
    mat4 mat_e = eigen_to_glm_mat4(frame->rotat_e);

    rotat_b = compute_anglesFromTransformationMatrix(mat_b);
    rotat_e = compute_anglesFromTransformationMatrix(mat_e);

    time_slam = frame->time_slam;
  }

  //SLAM time computation
  ImGui::Text("Computation:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.2f ms", time_slam);

  //SLAM results
  ImGui::Text("Tb [m]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", trans_b(0), trans_b(1), trans_b(2));
  ImGui::Text("Te [m]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", trans_e(0), trans_e(1), trans_e(2));
  ImGui::Text("Rb [°]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", rotat_b.x, rotat_b.y, rotat_b.z);
  ImGui::Text("Re [°]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", rotat_e.x, rotat_e.y, rotat_e.z);

  //---------------------------
}

vec3 GUI_Slam::compute_anglesFromTransformationMatrix(const mat4& mat){
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
