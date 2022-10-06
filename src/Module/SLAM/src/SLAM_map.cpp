#include "SLAM_map.h"

#include "../SLAM.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Data/struct_voxelMap.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"


//Constructor / Destructor
SLAM_map::SLAM_map(SLAM* slam){
  //---------------------------

  Engine_node* node_engine = slam->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->local_map = new slamap();

  //---------------------------
  this->update_configuration();
}
SLAM_map::~SLAM_map(){}

//Main function
void SLAM_map::update_configuration(){
  //---------------------------

  local_map->voxel_width = 1.0f;
  local_map->voxel_capacity = 20;
  local_map->linked_cloud_ID = -1;
  local_map->linked_subset_ID = -1;
  local_map->current_frame_ID = 0;

  this->min_root_distance = 5.0f;
  this->max_root_distance = 100.0f;
  this->max_voxel_distance = 150.0f;
  this->min_voxel_distance = 0.05;
  this->grid_voxel_width = 1;
  this->max_total_point = 20000;

  local_map->rotat_b = Eigen::Matrix3d::Identity();
  local_map->rotat_e = Eigen::Matrix3d::Identity();
  local_map->trans_b = Eigen::Vector3d::Zero();
  local_map->trans_e = Eigen::Vector3d::Zero();

  //---------------------------
}
void SLAM_map::compute_grid_sampling(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Clear vectors
  frame->xyz.clear();
  frame->xyz_raw.clear();
  frame->ts_n.clear();

  //Subsample the scan with voxels
  gridMap grid;
  Eigen::Vector4d point;
  for(int i=0; i<subset->xyz.size(); i++){
    vec3& xyz = subset->xyz[i];
    double& ts_n = subset->ts_n[i];

    double dist = fct_distance_origin(xyz);

    if(dist > min_root_distance && dist < max_root_distance){
      int kx = static_cast<int>(xyz.x / grid_voxel_width);
      int ky = static_cast<int>(xyz.y / grid_voxel_width);
      int kz = static_cast<int>(xyz.z / grid_voxel_width);
      int key = local_map->get_signature(kx, ky, kz);

      point << xyz.x, xyz.y, xyz.z, ts_n;
      grid[key].push_back(point);
    }
  }

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++){
    if(it->second.size() != 0){
      //Take one random point
      int rdm = rand() % it->second.size();

      Eigen::Vector4d point = it->second[0];
      Eigen::Vector3d xyz(point(0), point(1), point(2));

      frame->xyz.push_back(xyz);
      frame->ts_n.push_back(point(3));

      if(frame->xyz.size() > max_total_point){
        break;
      }
    }
  }

  frame->xyz_raw = frame->xyz;

  //---------------------------
}
void SLAM_map::update_map(Cloud* cloud, int subset_ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m = sceneManager->get_frame_byID(cloud, subset_ID - 5);
  //---------------------------

  if(subset_ID > 5){
    this->update_map_parameter(frame_m);
  }

  this->add_pointsToLocalMap(frame);
  this->end_clearTooFarVoxels(frame->trans_e);

  //---------------------------
}
void SLAM_map::update_map_parameter(Frame* frame){
  //---------------------------

  local_map->rotat_b = frame->rotat_b;
  local_map->trans_b = frame->trans_b;
  local_map->rotat_e = frame->rotat_e;
  local_map->trans_e = frame->trans_e;

  //---------------------------
}
void SLAM_map::reset_map_smooth(){
  //---------------------------

  local_map->map.clear();
  local_map->current_frame_ID = 0;
  local_map->linked_subset_ID = -1;
  local_map->current_frame_ID = 0;

  //---------------------------
}
void SLAM_map::reset_map_hard(){
  //---------------------------

  local_map->map.clear();
  local_map->linked_cloud_ID = -1;
  local_map->linked_subset_ID = -1;
  local_map->current_frame_ID = 0;

  local_map->rotat_b = Eigen::Matrix3d::Identity();
  local_map->rotat_e = Eigen::Matrix3d::Identity();
  local_map->trans_b = Eigen::Vector3d::Zero();
  local_map->trans_e = Eigen::Vector3d::Zero();

  //---------------------------
}

//sub-function
void SLAM_map::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d& point = frame->xyz[i];

    int kx = static_cast<int>(point(0) / local_map->voxel_width);
    int ky = static_cast<int>(point(1) / local_map->voxel_width);
    int kz = static_cast<int>(point(2) / local_map->voxel_width);
    int key = local_map->get_signature(kx, ky, kz);
    voxelMap_it it = local_map->map.find(key);

    //if the voxel already exists
    if(it != local_map->map.end()){
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = it.value();

      //If the voxel is not full
      if(voxel_xyz.size() < local_map->voxel_capacity){
        //Check if minimal distance with voxel points is respected
        double dist_min = 10000;
        for(int j=0; j<voxel_xyz.size(); j++){
          Eigen::Vector3d& voxel_point = voxel_xyz[j];
          double dist = fct_distance(point, voxel_point);
          if(dist < dist_min){
            dist_min = dist;
          }
        }

        //If all conditions are fullfiled, add the point to local map
        if(dist_min > min_voxel_distance){
          voxel_xyz.push_back(point);
        }
      }
    }
    //else create it
    else{
      vector<Eigen::Vector3d> voxel;
      voxel.push_back(point);
      local_map->map.insert({key, voxel});
    }
  }

  //---------------------------
}
void SLAM_map::end_clearTooFarVoxels(Eigen::Vector3d &current_location){
  vector<int> voxels_to_erase;
  //---------------------------

  for(auto it = local_map->map.begin(); it != local_map->map.end(); ++it){
    Eigen::Vector3d voxel_point = it->second[0];
    double dist = fct_distance(voxel_point, current_location);

    if(dist > max_voxel_distance){
      voxels_to_erase.push_back(it->first);
    }

  }

  for(int i=0; i<voxels_to_erase.size(); i++){
    local_map->map.erase(voxels_to_erase[i]);
  }

  //---------------------------
}
