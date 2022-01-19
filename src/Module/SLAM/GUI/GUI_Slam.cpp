#include "GUI_Slam.h"

#include "../CT_ICP.h"
#include "../CT_ICP/SLAM_optim_ceres.h"
#include "../CT_ICP/SLAM_optim_gn.h"
#include "../CT_ICP/SLAM_normal.h"

#include "../../../Engine/Data/Database.h"
#include "../../../Engine/Scene.h"
#include "../../../Engine/Engine.h"
#include "../../../Specific/fct_transtypage.h"

extern struct Database database;


//Constructor / Destructor
GUI_Slam::GUI_Slam(Engine* engineManager){
  //---------------------------

  this->cticpManager = engineManager->get_cticpManager();
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
    this->parameters_optimization();
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
    ImGui::Checkbox("Visual voxelized map", slamMap_voxelized);

    //Width of the SLAM map voxel
    if(*slamMap_voxelized){
      float* slamMap_width = cticpManager->get_voxel_size_slamMap();
      ImGui::SetNextItemWidth(item_width);
      ImGui::InputFloat("Voxel size voxelized map", slamMap_width, 0.1f, 1.0f, "%.3f");
    }

    //Number of threads
    static int nb_thread = 8;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number thread", &nb_thread, 0, 20)){
      SLAM_normal* normalManager = cticpManager->get_SLAM_normal();
      cticpManager->set_nb_thread(nb_thread);
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of threads for optimization and normal computation");
    }

    //Subsampling voxel width
    float* sampling_width = cticpManager->get_voxel_size_gridMap();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Grid sampling voxel size", sampling_width, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The voxel size for the grid sampling of the new frame (before keypoints extraction)");
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

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameters_optimization(){
  if(ImGui::TreeNode("Optimization")){
    Cloud* cloud = database.cloud_selected;
    //---------------------------

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

    //Maximum point to plane distance for optimization
    float* PTP_distance_max = gnManager->get_PTP_distance_max();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max PTP distance", PTP_distance_max, 0.01f, 4.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Maximum point to plane distance for optimization");
    }

    //Minimal requiered number of residual
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

    //Minimal optimization score
    float* thres_optimMinNorm = cticpManager->get_thres_optimMinNorm();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Minimal threshold", thres_optimMinNorm, 0.001f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Minimal optimization score to obtain to validate ICP");
    }

    //Maximum displacement in a frame
    float* thres_ego_trans = cticpManager->get_thres_ego_trans();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max ego translation", thres_ego_trans, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The motion of the sensor between two frames which is considered erroneous");
    }

    //Maximum rotation in a frame
    float* thres_ego_rotat = cticpManager->get_thres_ego_rotat();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max ego rotation", thres_ego_rotat, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The motion of the sensor between two frames which is considered erroneous");
    }

    //Maximum displacement between two poses
    float* thres_pose_trans = cticpManager->get_thres_pose_trans();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max pose translation", thres_pose_trans, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on distance changes (in m) for early termination of the ICP");
    }

    //Maximum rotation between two poses
    float* thres_pose_rotat = cticpManager->get_thres_pose_rotat();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Max pose rotation", thres_pose_rotat, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on orientation changes (in degrees) for early termination of the ICP");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::statistics(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  float time = 0;
  vec3 trans_abs = vec3(0, 0, 0);
  vec3 rotat_abs = vec3(0, 0, 0);
  vec3 trans_rlt = vec3(0, 0, 0);
  vec3 rotat_rlt = vec3(0, 0, 0);
  int map_size_abs = 0;
  int map_size_rlt = 0;

  if(cloud != nullptr){
    Frame* frame = &cloud->subset[cloud->subset_selected].frame;

    time = frame->time_slam;
    trans_abs = frame->trans_abs;
    rotat_abs = frame->rotat_abs;
    trans_rlt = frame->trans_rlt;
    rotat_rlt = frame->rotat_rlt;
    map_size_abs = frame->map_size_abs;
    map_size_rlt = frame->map_size_rlt;
  }

  //SLAM time computation
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Time");
  ImGui::Text("Computation:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.2f ms", time);

  //SLAM results - absolut transformation parameters
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Transformation");
  ImGui::Text("T abs [m]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", trans_abs.x, trans_abs.y, trans_abs.z);
  ImGui::Text("R abs [°]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", rotat_abs.x, rotat_abs.y, rotat_abs.z);

  //SLAM results - relative transformation parameters
  ImGui::Text("T rlt [m]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", trans_rlt.x, trans_rlt.y, trans_rlt.z);
  ImGui::Text("R rlt [°]:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f %.3f %.3f", rotat_rlt.x, rotat_rlt.y, rotat_rlt.z);

  //Local map data
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Local map");
  ImGui::Text("Nb voxel abs: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", map_size_abs);
  ImGui::Text("Nb voxel rlt: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", map_size_rlt);

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
