#include "GUI_Slam.h"

#include "../CT_ICP.h"
#include "../CT_ICP/SLAM_optim_ceres.h"
#include "../CT_ICP/SLAM_optim_gn.h"
#include "../CT_ICP/SLAM_normal.h"

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
  this->normalManager = cticpManager->get_SLAM_normal();

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
      cticpManager->compute_slam(cloud);

      sceneManager.update_cloud_location(cloud);
      sceneManager.update_cloud_glyphs(cloud);
    }

  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Slam::parameters(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    this->parameters_general();
    this->parameters_computation();
    this->parameters_localMap();
    this->parameters_normal();
    this->parameters_robustesse();

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Slam::parameters_general(){
  if(ImGui::TreeNode("General")){
    Cloud* cloud = database.cloud_selected;
    //---------------------------

    //Display infos in terminal
    bool* slam_verbose = cticpManager->get_verbose();
    ImGui::Checkbox("Verbose", slam_verbose);

    //Make a voxelized slam map
    bool* slamMap_voxelized = cticpManager->get_slamMap_voxelized();
    ImGui::Checkbox("Voxelized map", slamMap_voxelized);

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameters_computation(){
  if(ImGui::TreeNode("Computation")){
    Cloud* cloud = database.cloud_selected;
    //---------------------------

    //Subsampling voxel width
    float* sampling_width = cticpManager->get_voxel_size_gridMap();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Grid sampling voxel size", sampling_width, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The voxel size for the grid sampling of the new frame (before keypoints extraction)");
    }

    //Width of the SLAM map voxel
    float* slamMap_width = cticpManager->get_voxel_size_slamMap();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("SLAM map voxel size", slamMap_width, 0.1f, 1.0f, "%.3f");

    //Number of optimization iterations
    static int iter_max = 5;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number iter", &iter_max, 1, 20)){
      ceresManager->set_iter_max(iter_max);
      gnManager->set_iter_max(iter_max);
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of iteration for the optimization algorithm");
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
      cticpManager->set_nb_thread(nb_thread);
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of threads for the optimization algorithm");
    }

    //Maximum point to plane distance for optimization
    float* PTP_distance_max = gnManager->get_PTP_distance_max();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max PTP distance", PTP_distance_max, 0.01f, 4.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Maximum point to plane distance for optimization");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameters_localMap(){
  if(ImGui::TreeNode("Local map")){
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Frame");

    //Subset point minimum distance
    float* min_subset_distance = cticpManager->get_min_subset_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Min point distance from LiDAR", min_subset_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to close to the center of the sensor");
    }

    //Subset point maximum distance
    float* max_subset_distance = cticpManager->get_max_subset_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max point distance from LiDAR", max_subset_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to far from the center of the sensor");
    }

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Voxel");

    //Width of the local map voxel
    float* localMap_width = cticpManager->get_voxel_size_localMap();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Voxel width", localMap_width, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The size of a voxel for the selection of `keypoints` by grid sampling");
    }

    //Mnimun distance between points inside a voxel
    float* min_voxel_distance = cticpManager->get_min_voxel_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Min point dist voxel ", min_voxel_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Mnimun distance between points inside a voxel");
    }

    //Distance threshold to supress the voxels on run
    float* max_voxel_distance = cticpManager->get_max_voxel_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max dist from position", max_voxel_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Distance threshold to supress the voxels on run");
    }

    //Number of point per voxel
    int* nb_points_per_voxel = cticpManager->get_map_max_voxelNbPoints();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Number point per voxel", nb_points_per_voxel, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The maximum number of points per voxel of the map");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameters_normal(){
  if(ImGui::TreeNode("Normal")){
    //---------------------------

    //Number k nearest neighbors
    int* max_number_neighbors = normalManager->get_max_number_neighbors();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Max number kNN", max_number_neighbors, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("   ");
    }

    //Number of voxel to search kNN
    int* voxel_searchSize = normalManager->get_voxel_searchSize();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Voxel search number", voxel_searchSize, 1, 10);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of voxel to search kNN in each dimension and in two directions");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameters_robustesse(){
  if(ImGui::TreeNode("Robustesse")){
    //---------------------------

    //Maximum displacement between two poses
    float* thres_min_distance = cticpManager->get_thres_min_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max distance movement", thres_min_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The motion of the sensor between two frames which is considered erroneous");
    }

    //Translation diff
    float* thres_trans_norm = cticpManager->get_thres_trans_norm();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max translation norm", thres_trans_norm, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on distance changes (in m) for early termination of the ICP");
    }

    //Rotation diff
    float* thres_rotat_norm = cticpManager->get_thres_rotat_norm();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max rotation norm", thres_rotat_norm, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on orientation changes (in degrees) for early termination of the ICP");
    }

    //Number of optimization iterations
    int* nb_residual_min = gnManager->get_nb_residual_min();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Minimal number residual", nb_residual_min, 10, 500);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The minimum number of residuals for a valid ICP problem");
    }


    //---------------------------
    ImGui::TreePop();
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
