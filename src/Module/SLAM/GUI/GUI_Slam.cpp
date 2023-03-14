#include "GUI_Slam.h"

#include "../Module_slam.h"
#include "../ct_icp/CT_ICP.h"
#include "../Glyph/SLAM_glyph.h"
#include "../src/SLAM.h"
#include "../src/SLAM_assessment.h"
#include "../src/SLAM_map.h"
#include "../src/SLAM_parameter.h"
#include "../src/SLAM_transform.h"
#include "../src/SLAM_sampling.h"

#include "../optim/SLAM_optim.h"
#include "../optim/SLAM_optim_ceres.h"
#include "../optim/SLAM_optim_gn.h"
#include "../optim/SLAM_normal.h"

#include "../../Node_module.h"

#include "../../../GUI/Node_gui.h"
#include "../../../Engine/Core/Engine.h"
#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Specific/Function/fct_transtypage.h"


//Constructor / Destructor
GUI_Slam::GUI_Slam(Module_slam* module){
  this->module_slam = module;
  //---------------------------

  Node_module* node_module = module_slam->get_node_module();
  Node_engine* node_engine = node_module->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->slamManager = module_slam->get_slamManager();
  this->slam_optim = slamManager->get_slam_optim();
  this->slam_optim_gn = slam_optim->get_optim_gn();
  this->slam_normal = slamManager->get_slam_normal();
  this->slam_assess = slamManager->get_slam_assess();
  this->slam_map = slamManager->get_slam_map();
  this->slam_param = slamManager->get_slam_param();
  this->slam_transf = slamManager->get_slam_transf();
  this->slam_glyph = slamManager->get_slam_glyph();
  this->slam_sampling = slamManager->get_slam_sampling();
  this->cticpManager = module_slam->get_cticpManager();

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
      this->design_misc();

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
void GUI_Slam::design_misc(){
  if(ImGui::BeginTabItem("Misc##123")){
    Collection* collection = sceneManager->get_selected_collection();
    //---------------------------

    ImGui::PushItemWidth(100);
    if(ImGui::Button("CT-ICP", ImVec2(75,0))){
      cticpManager->algo(collection);
      //sceneManager->update_buffer_location(cloud);
    }

    //---------------------------
    ImGui::EndTabItem();
  }
}
void GUI_Slam::design_parameter(){
  if(ImGui::BeginTabItem("Parameter##123")){
    //---------------------------

    this->parameter_lidar();
    this->parameter_algo();

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

  bool* with_slam = module_slam->get_with_slam();
  ImGui::Checkbox("SLAM", with_slam);

  //---------------------------
}

void GUI_Slam::parameter_lidar(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //Configuration model
  int slam_conf;
  if(collection == nullptr){
    slam_conf = *slam_param->get_predefined_conf();
  }else{
    if(collection->lidar_model == "velodyne_vlp16"){
      slam_conf = 0;
    }
    else if(collection->lidar_model == "velodyne_vlp64"){
      slam_conf = 1;
    }
    else if(collection->lidar_model == "velodyne_hdl32"){
      slam_conf = 2;
    }
    else if(collection->lidar_model == "velodyne_vlp16_reduced"){
      slam_conf = 3;
    }
  }

  ImGui::SetNextItemWidth(item_width);
  if(ImGui::Combo("LiDAR", &slam_conf, "vlp_16\0vlp_64\0hdl_32\0vlp_16_reduced\0")){
    slam_param->set_predefined_conf(slam_conf);

    if(slam_conf == 0){
      collection->lidar_model = "velodyne_vlp16";
    }
    else if(slam_conf == 1){
      collection->lidar_model = "velodyne_vlp64";
    }
    else if(slam_conf == 2){
      collection->lidar_model = "velodyne_hdl32";
    }
    else if(slam_conf == 3){
      collection->lidar_model = "velodyne_vlp16_reduced";
    }
  }

  //---------------------------
}
void GUI_Slam::parameter_algo(){
  //---------------------------

  int* algo = module_slam->get_algo();
  ImGui::SetNextItemWidth(item_width);
  ImGui::Combo("Algo##444", algo, "ct_icp\0custom\0");

  //---------------------------
  ImGui::Separator();
}
void GUI_Slam::parameter_glyph(){
  if(ImGui::TreeNode("Glyph##tree")){
    ImGui::Columns(2);
    //---------------------------

    //Keypoint
    bool* with_keypoint = slam_glyph->get_with_keypoint();
    ImGui::Checkbox("Keypoint##3", with_keypoint);
    ImGui::NextColumn();

    //Neighbor
    bool* with_neighbor = slam_glyph->get_with_neighbor();
    ImGui::Checkbox("Neighbor##3", with_neighbor);
    ImGui::NextColumn();

    //Matching
    bool* with_matching = slam_glyph->get_with_matching();
    ImGui::Checkbox("Matching##3", with_matching);
    ImGui::NextColumn();

    //Car
    bool* with_car = slam_glyph->get_with_car();
    ImGui::Checkbox("Car", with_car);
    ImGui::NextColumn();

    //Trajectory
    bool* with_trajectory = slam_glyph->get_with_trajectory();
    ImGui::Checkbox("Trajectory", with_trajectory);
    ImGui::NextColumn();

    //Local map
    bool* with_localmap = slam_glyph->get_with_localmap();
    ImGui::Checkbox("Local map", with_localmap);
    ImGui::NextColumn();

    //---------------------------
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_offline(){
  if(ImGui::TreeNode("Offline##tree")){
    Collection* collection = sceneManager->get_selected_collection();
    //---------------------------

    //Compute algorithm
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Compute", ImVec2(item_width,0))){
      if(collection != nullptr){
        sceneManager->reset_collection(collection);
        //slamManager->compute_slam_offline(cloud);

        //sceneManager->update_buffer_location(cloud);
        sceneManager->update_glyph(collection);
      }
    }
    ImGui::PopStyleColor(1);

    //Number of frame to compute
    if(collection != nullptr){
      static int frame_max = collection->nb_obj;
      ImGui::SetNextItemWidth(item_width);
      if(ImGui::SliderInt("Number frame", &frame_max, 1, collection->nb_obj)){
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
    double* PTP_distance_min = slam_optim_gn->get_dist_residual_min();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Min PTP distance", PTP_distance_min, 0.01f, 4.0f, "%.3f")){
      if(*PTP_distance_min < 0){
        *PTP_distance_min = 0;
      }
    }
    double* PTP_distance_max = slam_optim_gn->get_dist_residual_max();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Max PTP distance", PTP_distance_max, 0.1f, 4.0f, "%.3f")){
      if(*PTP_distance_max < 0){
        *PTP_distance_max = 0;
      }
    }
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

    //Lambda
    double* lambda_location = slam_optim_gn->get_lambda_location_consistency();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Lambda location consistency", lambda_location, 0.001f, 4.0f, "%.3f")){
      if(*lambda_location < 0.001){
        *lambda_location = 0;
      }
    }

    double* lambda_velocity = slam_optim_gn->get_lambda_constant_velocity();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Lambda constant velocity", lambda_velocity, 0.001f, 4.0f, "%.3f")){
      if(*lambda_velocity < 0.001){
        *lambda_velocity = 0;
      }
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void GUI_Slam::parameter_gridSampling(){
  if(ImGui::TreeNode("Sampling##tree")){
    //---------------------------

    //Subset point minimum distance
    double* min_root_distance = slam_sampling->get_min_root_distance();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Min point distance from LiDAR", min_root_distance, 0.1f, 1.0f, "%.3f")){
      if(*min_root_distance < 0){
        *min_root_distance = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to close to the center of the sensor");
    }

    //Subset point maximum distance
    double* max_root_distance = slam_sampling->get_max_root_distance();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Max point distance from LiDAR", max_root_distance, 0.1f, 1.0f, "%.3f")){
      if(*max_root_distance < 0){
        *max_root_distance = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Suppresses points to far from the center of the sensor");
    }

    //Subsampling voxel width
    double* grid_voxel_size = slam_sampling->get_grid_voxel_size();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Grid sampling voxel size", grid_voxel_size, 0.1f, 1.0f, "%.3f")){
      if(*grid_voxel_size < 0){
        *grid_voxel_size = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The voxel size for the grid sampling of the new frame (before keypoints extraction)");
    }

    //Max keypoints
    int* max_keypoint = slam_sampling->get_max_keypoint();
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
    if(ImGui::InputDouble("Voxel width", localMap_width, 0.1f, 1.0f, "%.3f")){
      if(*localMap_width < 0){
        *localMap_width = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("The size of a voxel for the selection of `keypoints` by grid sampling");
    }

    //Mnimun distance between points inside a voxel
    double* min_dist_point_in_voxel = &local_map->voxel_min_point_dist;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Min point dist in voxel", min_dist_point_in_voxel, 0.01f, 10.0f, "%.3f")){
      if(*min_dist_point_in_voxel < 0){
        *min_dist_point_in_voxel = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Mnimun distance between points inside a voxel");
    }

    //Distance threshold to supress the voxels on run
    double* max_voxel_distance = &local_map->voxel_max_dist;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::InputDouble("Voxel max dist", max_voxel_distance, 1, 1.0f, "%.3f")){
      if(*max_voxel_distance < 0){
        *max_voxel_distance = 0;
      }
    }
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Distance threshold to supress the voxels on run");
    }

    //Number of point per voxel
    int* nb_points_per_voxel = &local_map->voxel_capacity;
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("Number point per voxel", nb_points_per_voxel, 1, 5000);
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
    ImGui::Checkbox("Active##44", with_local_cloud);
    ImGui::SameLine();

    // Visibility
    bool* with_localcloud = slam_glyph->get_with_localcloud();
    ImGui::Checkbox("Visible##44", with_localcloud);

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
    if(ImGui::InputDouble("Voxel width", voxel_width, 0.1f, 10, "%.2f")){
      if(*voxel_width < 0){
        *voxel_width = 0;
      }
    }
    ImGui::DragInt("Voxel capacity", voxel_capacity, 1, 1, 1000, "%d");
    if(ImGui::InputDouble("Voxel max dist", voxel_max_dist, 1, 1000, "%.2f")){
      if(*voxel_max_dist < 0){
        *voxel_max_dist = 0;
      }
    }

    //Local cloud color
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;
    Glyph* localcloud = slam_glyph->get_glyph_byName("localcloud");
    vec4* rgb = &localcloud->unicolor;
    ImGui::ColorEdit4("Color", (float*)rgb, flags);
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

    //Maximal computation time
    int* thres_time = slam_assess->get_thres_time_ms();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputInt("Maximal time [ms]", thres_time, 10);
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Maximal computation time");
    }

    //Minimal optimization score
    double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Minimal threshold", thres_optimMinNorm, 0.1f, 1.0f, "%.3f");
    if(ImGui::IsItemHovered()){
      ImGui::SetTooltip("Minimal optimization score to obtain to validate ICP");
    }

    //Maximal X and Y axis rotation angle
    double* thres_diff_angle = slam_assess->get_thres_diff_angle();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputDouble("Max X and Y axis diff angle", thres_diff_angle, 0.1f, 3.0f, "%.1f");
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

  bool is_slam_made = false;
  float time = 0;

  if(sceneManager->get_is_list_empty() == false){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    Frame* frame = &cloud->frame;

    is_slam_made = frame->is_slam_made;
    time = frame->time_slam;
  }

  //SLAM applied
  ImGui::Text("Applied:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_slam_made ? "Yes" : "No");
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
  int nb_residual_false = 0;
  int nb_keypoint = 0;

  if(sceneManager->get_is_list_empty() == false){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    Frame* frame = &cloud->frame;

    map_size_abs = frame->map_size_abs;
    map_size_rlt = frame->map_size_rlt;
    nb_residual = frame->nb_residual;
    nb_residual_false = frame->nb_residual_false;
    nb_keypoint = frame->xyz.size();
  }

  //Local map data
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Local map");
  ImGui::Text("Nb keypoint: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", nb_keypoint);
  ImGui::Text("Nb residual: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", nb_residual);
  ImGui::Text("Nb false residual: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", nb_residual_false);
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
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    Frame* frame = &cloud->frame;

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
