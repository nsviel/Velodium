#include "SLAM_localMap.h"

#include "../../../Engine/Data/struct_voxelMap.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"


//Constructor / Destructor
SLAM_localMap::SLAM_localMap(){
  //---------------------------

  this->map_local = new voxelMap();
  this->map_cloud = new slamMap();

  //---------------------------
  this->update_configuration();
}
SLAM_localMap::~SLAM_localMap(){}

//Main function
void SLAM_localMap::update_configuration(){
  //---------------------------

  this->map_voxel_size = 1;
  this->slamMap_voxel_size = 0.5;

  this->min_root_distance = 5.0f;
  this->max_root_distance = 100.0f;
  this->max_voxel_distance = 150.0f;
  this->map_voxel_capacity = 20;
  this->min_voxel_distance = 0.05;
  this->grid_sampling_voxel_size = 1;
  this->max_total_point = 20000;

  this->slamMap_voxelized = false;

  //---------------------------
}
void SLAM_localMap::reset_map(){
  //---------------------------

  this->map_local->clear();
  this->map_cloud->clear();

  //---------------------------
}

void SLAM_localMap::compute_grid_sampling(Subset* subset){
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
    double dist = fct_distance_origin(xyz);

    if(dist > min_root_distance && dist < max_root_distance){
      int kx = static_cast<int>(xyz.x / grid_sampling_voxel_size);
      int ky = static_cast<int>(xyz.y / grid_sampling_voxel_size);
      int kz = static_cast<int>(xyz.z / grid_sampling_voxel_size);
      int key = retrieve_map_signature(kx, ky, kz);

      point << xyz.x, xyz.y, xyz.z, subset->ts_n[i];
      grid[key].push_back(point);
    }
  }

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++){
    if(it->second.size() != 0){
      //Take one random point
      int rdm = rand() % it->second.size();

      Eigen::Vector4d point = it->second[rdm];
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
void SLAM_localMap::add_pointsToSlamMap(Subset* subset){
  //---------------------------

  //Insert points into cloud global slam map
  if(slamMap_voxelized){
    for (int i=0; i<subset->xyz.size(); i++){
      vec3& xyz = subset->xyz[i];

      int kx = static_cast<int>(xyz.x / slamMap_voxel_size);
      int ky = static_cast<int>(xyz.y / slamMap_voxel_size);
      int kz = static_cast<int>(xyz.z / slamMap_voxel_size);
      int key = retrieve_map_signature(kx, ky, kz);

      //if the voxel already exists
      if(map_cloud->find(key) != map_cloud->end()){
        //Get corresponding voxel
        vector<vec3>& voxel_xyz = map_cloud->find(key).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_voxel_capacity){
          voxel_xyz.push_back(xyz);
          subset->xyz_voxel.push_back(xyz);
        }
      }
      //else create it
      else{
        vector<vec3> vec;
        vec.push_back(xyz);
        subset->xyz_voxel.push_back(xyz);
        map_cloud->insert({key, vec});
      }
    }
  }

  //---------------------------
}
void SLAM_localMap::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d& point = frame->xyz[i];

    int kx = static_cast<int>(point(0) / map_voxel_size);
    int ky = static_cast<int>(point(1) / map_voxel_size);
    int kz = static_cast<int>(point(2) / map_voxel_size);
    int key = retrieve_map_signature(kx, ky, kz);

    voxelMap_it it = map_local->find(key);

    //if the voxel already exists
    if(it != map_local->end()){
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = it.value();

      //If the voxel is not full
      if(voxel_xyz.size() < map_voxel_capacity){
        //Check if minimal distance with voxel points is respected
        double dist_min = 10000;
        for(int j=0; j<voxel_xyz.size(); j++){
          Eigen::Vector3d& voxel_point = voxel_xyz[j];
          double dist = fct_distance(point, voxel_point);
          if (dist < dist_min) {
            dist_min = dist;
          }
        }

        //If all conditions are fullfiled, add the point to local map
        if (dist_min > min_voxel_distance) {
          voxel_xyz.push_back(point);
        }
      }
    }
    //else create it
    else{
      vector<Eigen::Vector3d> voxel;
      voxel.push_back(point);
      map_local->insert({key, voxel});
    }
  }

  //---------------------------
}

void SLAM_localMap::end_slamVoxelization(Cloud* cloud, int frame_max){
  //---------------------------

  if(slamMap_voxelized){
    for(int i=0; i<frame_max; i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      subset->xyz = subset->xyz_voxel;
    }
  }

  //---------------------------
}
void SLAM_localMap::end_clearTooFarVoxels(Eigen::Vector3d &current_location){
  vector<int> voxels_to_erase;
  //---------------------------

  for(auto it = map_local->begin(); it != map_local->end(); ++it){
    Eigen::Vector3d voxel_point = it->second[0];
    double dist = fct_distance(voxel_point, current_location);

    if(dist > max_voxel_distance){
      voxels_to_erase.push_back(it->first);
    }

  }

  for(int i=0; i<voxels_to_erase.size(); i++){
    map_local->erase(voxels_to_erase[i]);
  }

  //---------------------------
}
