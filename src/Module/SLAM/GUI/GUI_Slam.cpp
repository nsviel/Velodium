#include "GUI_Slam.h"

#include "../Module_slam.h"
#include "../src/SLAM.h"
#include "../src/SLAM_assessment.h"
#include "../src/SLAM_map.h"
#include "../src/SLAM_parameter.h"
#include "../src/SLAM_transform.h"
#include "../optim/SLAM_optim.h"
#include "../optim/SLAM_optim_ceres.h"
#include "../optim/SLAM_optim_gn.h"
#include "../optim/SLAM_normal.h"

#include "../../Node_module.h"

#include "../../../GUI/Node_gui.h"
#include "../../../Engine/Core/Engine.h"
#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Object.h"
#include "../../../Engine/Scene/Object/SLAM/Car.h"
#include "../../../Engine/Scene/Object/SLAM/Trajectory.h"
#include "../../../Engine/Scene/Object/SLAM/Slam_keypoint.h"
#include "../../../Engine/Scene/Object/SLAM/Localmap.h"
#include "../../../Specific/fct_transtypage.h"


//Constructor / Destructor
GUI_Slam::GUI_Slam(Module_slam* module){
  //---------------------------

  Node_module* node_module = module->get_node_module();
  Node_engine* node_engine = node_module->get_node_engine();

  this->slamManager = module->get_slamManager();
  SLAM_optim* optimManager = slamManager->get_slam_optim();

  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->slam_optim_gn = optimManager->get_optim_gn();
  this->slam_normal = slamManager->get_slam_normal();
  this->slam_assess = slamManager->get_slam_assess();
  this->slam_map = slamManager->get_slam_map();
  this->slam_param = slamManager->get_slam_param();
  this->slam_transf = slamManager->get_slam_transf();

  this->item_width = 100;

  //---------------------------
}
GUI_Slam::~GUI_Slam(){}

//Main function
void GUI_Slam::design_SLAM(){
  if(ImGui::BeginTabItem("SLAM##hello")){
    if(ImGui::BeginTabBar("##tabs_slam", ImGuiTabBarFlags_None)){
      //---------------------------

      this->design_parameter();
      this->design_state();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Slam::design_state(){
  if(ImGui::BeginTabItem("State##123")){
    //---------------------------

    this->state_SLAM();
    this->state_localmap();
    this->state_transformation();

    //---------------------------
    ImGui::EndTabItem();
  }
}
void GUI_Slam::design_parameter(){
  if(ImGui::BeginTabItem("Parameter##123")){
    //---------------------------

    this->parameter_lidar();
    this->parameter_glyph();
    this->parameter_optimization();
    this->parameter_gridSampling();
    this->parameter_localMap();
    this->parameter_localCloud();
    this->parameter_normal();
    this->parameter_robustesse();

    //---------------------------
    ImGui::Separator();
    ImGui::EndTabItem();
  }
}
void GUI_Slam::design_option(){
  //---------------------------

  bool* with_slam = slamManager->get_with_slam();
  ImGui::Checkbox("SLAM", with_slam);

  //---------------------------
}

void GUI_Slam::parameter_lidar(){
  //---------------------------

  //Configuration model
  int slam_conf = *slam_param->get_predefined_conf();
  ImGui::SetNextItemWidth(item_width);
  if(ImGui::Combo("LiDAR", &slam_conf, "vlp_16\0vlp_64\0hdl_32\0train\0")){
    slam_param->set_predefined_conf(slam_conf);
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Slam::parameter_glyph(){
  if(ImGui::TreeNode("Glyph##tree")){
    ImGui::Columns(2);
    //---------------------------

    //Display ICP line correspondences
    Slam_keypoint* keyObject = objectManager->get_object_keypoint();
    bool* keypoint_ON = keyObject->get_visibility();
    if(ImGui::Checkbox("Keypoint", keypoint_ON)){
      if(sceneManager->get_is_list_empty() == false){
        objectManager->set_object_visibility("keypoint", *keypoint_ON);
      }
    }
    ImGui::NextColumn();

    //Car
    Car* carObject = objectManager->get_object_car();
    bool* car_visu = carObject->get_visibility();
    ImGui::Checkbox("Car", car_visu);
    ImGui::NextColumn();

    //Trajectory
    Trajectory* trajObject = objectManager->get_object_trajectory();
    bool* traj_visu = trajObject->get_visibility();
    ImGui::Checkbox("Trajectory", traj_visu);
    ImGui::NextColumn();

    //Local map
    Localmap* mapObject = objectManager->get_object_localmap();
    Glyph* localmap = mapObject->get_localmap();
    ImGui::Checkbox("Local map", &localmap->visibility);
    ImGui::NextColumn();

    //---------------------------
    ImGui::Columns(1);
    ImGui::Separator();
  }
}
void GUI_Slam::parameter_offline(){
  if(ImGui::TreeNode("Offline##tree")){
    Cloud* cloud = sceneManager->get_selected_cloud();
    //---------------------------

    //Compute algorithm
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Compute", ImVec2(item_width,0))){
      if(cloud != nullptr){
        sceneManager->reset_cloud(cloud);
        //slamManager->compute_slam_offline(cloud);

        sceneManager->update_cloud_location(cloud);
        sceneManager->update_cloud_glyph(cloud);
      }
    }
    ImGui::PopStyleColor(1);

    //Number of frame to compute
    if(cloud != nullptr){
      static int frame_max = cloud->nb_subset;
      ImGui::SetNextItemWidth(item_width);
      if(ImGui::SliderInt("Number frame", &frame_max, 1, cloud->nb_subset)){
        //slamManager->set_offline_ID_max(frame_max);
      }
    }else{
      static int frame_max = 0;
      ImGui::SetNextItemWidth(item_width);
      ImGui::SliderInt("Number frame", &frame_max, 0, 0);
    }

    //---------------------------
    ImGui::TreePop();
    ImGui::Separator();
  }
}
void GUI_Slam::parameter_optimization(){
  if(ImGui::TreeNode("Optimization##tree")){
    //---------------------------

    //Number of threads
    static int nb_thread = 8;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number thread", &nb_thread, 0, 20)){
      slam_param->set_nb_thread(nb_thread);
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of threads for optimization and normal computation");
    }

    //Number of optimization iterations
    static int iter_max = 5;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Number iter", &iter_max, 1, 20)){
      //slam_optim_ceres->set_iter_max(iter_max);
      slam_optim_gn->set_iter_max(iter_max);
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of iteration for the optimization algorithm");
    }

    //Maximum point to plane distance for optimization
    double* PTP_distance_max = slam_optim_gn->get_dist_residual_max();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max PTP distance", PTP_distance_max, 0.01f, 4.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Maximum point to plane distance for optimization");
    }

    //Minimal requiered number of residual
    int* nb_residual_min = slam_assess->get_nb_residual_min();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Minimal number residual", nb_residual_min, 10, 500);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The minimum number of residuals for a valid ICP problem");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_gridSampling(){
  if(ImGui::TreeNode("Grid sampling##tree")){
    //---------------------------

    //Subset point minimum distance
    double* min_root_distance = slam_transf->get_min_root_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Min point distance from LiDAR", min_root_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to close to the center of the sensor");
    }

    //Subset point maximum distance
    double* max_root_distance = slam_transf->get_max_root_distance();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max point distance from LiDAR", max_root_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to far from the center of the sensor");
    }

    //Subsampling voxel width
    double* grid_voxel_size = slam_transf->get_grid_voxel_size();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Grid sampling voxel size", grid_voxel_size, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The voxel size for the grid sampling of the new frame (before keypoints extraction)");
    }

    //Max keypoints
    int* max_keypoint = slam_transf->get_max_keypoint();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Max keypoint", max_keypoint, 100, 10000);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Max number of keypoint to take into account");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_localMap(){
  if(ImGui::TreeNode("Local map##tree")){
    slamap* local_map = slam_map->get_local_map();
    //---------------------------

    //Width of the local map voxel
    double* localMap_width = &local_map->voxel_width;
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Voxel width", localMap_width, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The size of a voxel for the selection of `keypoints` by grid sampling");
    }

    //Mnimun distance between points inside a voxel
    double* min_dist_point_in_voxel = &local_map->voxel_min_point_dist;
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Min point dist in voxel", min_dist_point_in_voxel, 0.001f, 10.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Mnimun distance between points inside a voxel");
    }

    //Distance threshold to supress the voxels on run
    double* max_voxel_distance = &local_map->voxel_max_dist;
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Voxel max dist", max_voxel_distance, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Distance threshold to supress the voxels on run");
    }

    //Number of point per voxel
    int* nb_points_per_voxel = &local_map->voxel_capacity;
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Number point per voxel", nb_points_per_voxel, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The maximum number of points per voxel of the map");
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_localCloud(){
  if(ImGui::TreeNode("Local cloud##tree")){
    slamap* local_cloud = slam_map->get_local_cloud();
    //---------------------------

    // With local cloud
    bool* with_local_cloud = slam_map->get_with_local_cloud();
    ImGui::Checkbox("Active", with_local_cloud);
    ImGui::SameLine();

    // Visibility
    Localmap* mapObject = objectManager->get_object_localmap();
    Glyph* glyph_localcloud = mapObject->get_localcloud();
    ImGui::Checkbox("Visible", &glyph_localcloud->visibility);

    // Save resulting cloud
    ImGui::PushItemWidth(100);
    if(ImGui::Button("Save##localcloud", ImVec2(75,0))){
      slam_map->save_local_cloud();
    }

    // Cloud map parameters
    double* voxel_width = &local_cloud->voxel_width;
    double* voxel_max_dist = &local_cloud->voxel_max_dist;
    int* voxel_capacity = &local_cloud->voxel_capacity;
    ImGui::PushItemWidth(100);
    ImGui::InputDouble("Voxel width", voxel_width, 0.1f, 10, "%.2f");
    ImGui::DragInt("Voxel capacity", voxel_capacity, 1, 1, 1000, "%d");
    ImGui::InputDouble("Voxel max dist", voxel_max_dist, 1, 1000, "%.2f");

    //Local cloud color
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs;
    flags |= ImGuiColorEditFlags_AlphaBar;
    vec4 rgb = glyph_localcloud->color_unique;
    if(ImGui::ColorEdit4("Color", (float*)&rgb, flags)){
      glyph_localcloud->color_unique = rgb;
    }
    ImGui::Separator();

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_normal(){
  if(ImGui::TreeNode("Normal##tree")){
    //---------------------------

    //Number k nearest neighbors
    int* max_number_neighbors = slam_normal->get_knn_max_nn();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Max number kNN", max_number_neighbors, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("   ");
    }

    //Number of voxel to search kNN
    int* voxel_searchSize = slam_normal->get_knn_voxel_search();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Voxel search number", voxel_searchSize, 1, 10);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of voxel to search kNN in each dimension and in two directions");
    }

    //kNN voxel size
    /*double* knn_voxel_capacity = slam_normal->get_knn_voxel_width();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("kNN voxel size", knn_voxel_capacity, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The voxel size for the kNN search");
    }*/

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_robustesse(){
  if(ImGui::TreeNode("Robustesse##tree")){
    //---------------------------

    //Minimal optimization score
    double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Minimal threshold", thres_optimMinNorm, 0.001f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Minimal optimization score to obtain to validate ICP");
    }

    //Maximal X and Y axis rotation angle
    double* thres_diff_angle = slam_assess->get_thres_diff_angle();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max X and Y axis diff angle", thres_diff_angle, 0.001f, 3.0f, "%.1f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Maximal X and Y axis rotation angle");
    }

    //Maximum displacement in a frame
    double* thres_ego_trans = slam_assess->get_thres_ego_trans();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max ego translation", thres_ego_trans, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The motion of the sensor between two frames which is considered erroneous");
    }

    //Maximum rotation in a frame
    double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max ego rotation", thres_ego_rotat, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The motion of the sensor between two frames which is considered erroneous");
    }

    //Maximum displacement between two poses
    double* thres_pose_trans = slam_assess->get_thres_pose_trans();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max pose translation", thres_pose_trans, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on distance changes (in m) for early termination of the ICP");
    }

    //Maximum rotation between two poses
    double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max pose rotation", thres_pose_rotat, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Threshold on orientation changes (in degrees) for early termination of the ICP");
    }

    //Number of previous pose taking into account
    int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("X previous rlt pose", nb_rlt_previous_pose, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of previous pose taking into account");
    }

    //Number of time threashold from mean previous pose
    int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("X time thres previous rlt pose", nb_rlt_previous_mean, 1, 100);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Number of time threashold from mean previous pose");
    }

    //---------------------------
    ImGui::TreePop();
  }
}

void GUI_Slam::state_SLAM(){
  //---------------------------

  bool is_slamed = false;
  float time = 0;

  if(sceneManager->get_is_list_empty() == false){
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    Frame* frame = &subset->frame;

    is_slamed = frame->is_slamed;
    time = frame->time_slam;
  }

  //SLAM applied
  ImGui::Text("Applied:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_slamed ? "Yes" : "No");
  ImGui::Text("Computation time:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.2f ms", time);

  //---------------------------
}
void GUI_Slam::state_localmap(){
  //---------------------------

  int map_size_abs = 0;
  int map_size_rlt = 0;
  int nb_residual = 0;
  int nb_keypoint = 0;

  if(sceneManager->get_is_list_empty() == false){
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    Frame* frame = &subset->frame;

    map_size_abs = frame->map_size_abs;
    map_size_rlt = frame->map_size_rlt;
    nb_residual = frame->nb_residual;
    nb_keypoint = subset->keypoint.location.size();
  }

  //Local map data
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Local map");
  ImGui::Text("Nb keypoint: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", nb_keypoint);
  ImGui::Text("Nb residual: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", nb_residual);
  ImGui::Text("Nb voxel abs: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", map_size_abs);
  ImGui::Text("Nb voxel rlt: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", map_size_rlt);

  //---------------------------
}
void GUI_Slam::state_transformation(){
  //---------------------------

  vec3 trans_b = vec3(0, 0, 0);
  vec3 rotat_b = vec3(0, 0, 0);
  vec3 trans_e = vec3(0, 0, 0);
  vec3 rotat_e = vec3(0, 0, 0);

  vec3 trans_b_rlt = vec3(0, 0, 0);
  vec3 rotat_b_rlt = vec3(0, 0, 0);
  vec3 trans_e_rlt = vec3(0, 0, 0);
  vec3 rotat_e_rlt = vec3(0, 0, 0);

  if(sceneManager->get_is_list_empty() == false){
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    Frame* frame = &subset->frame;

    trans_b_rlt = frame->trans_b_rlt;
    rotat_b_rlt = frame->rotat_b_rlt;
    trans_e_rlt = frame->trans_e_rlt;
    rotat_e_rlt = frame->rotat_e_rlt;

    trans_b = vec3(frame->trans_b(0), frame->trans_b(1), frame->trans_b(2));
    rotat_b = frame->angle_b;
    trans_e = vec3(frame->trans_e(0), frame->trans_e(1), frame->trans_e(2));
    rotat_e = frame->angle_e;
  }

  //SLAM results - Ego transformation parameters
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Transformation");
  display_stat("T_b [m]:", trans_b, trans_b_rlt);
  display_stat("R_b [°]:", rotat_b, rotat_b_rlt);

  display_stat("T_e [m]:", trans_e, trans_e_rlt);
  display_stat("R_e [°]:", rotat_e, rotat_e_rlt);

  //---------------------------
}
void GUI_Slam::display_stat(string title, vec3 abs, vec3 rlt){
  //---------------------------

  ImGui::Text("%s", title.c_str());
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%.3f (", abs.x);
  ImGui::SameLine();
  if(rlt.x >= 0){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "+%.3f", rlt.x);
  }else{
    ImGui::TextColored(ImVec4(1.0f,0.5f,0.0f,1.0f), "-%.3f", rlt.x);
  }
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), ") %.3f (", abs.y);
  ImGui::SameLine();
  if(rlt.y >= 0){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "+%.3f", rlt.y);
  }else{
    ImGui::TextColored(ImVec4(1.0f,0.5f,0.0f,1.0f), "-%.3f", rlt.y);
  }
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), ") %.3f (", abs.z);
  ImGui::SameLine();
  if(rlt.z >= 0){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "+%.3f", rlt.z);
  }else{
    ImGui::TextColored(ImVec4(1.0f,0.5f,0.0f,1.0f), "-%.3f", rlt.z);
  }
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), ")");

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
