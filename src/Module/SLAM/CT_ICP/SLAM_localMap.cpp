#include "SLAM_localMap.h"

#include "../../../Engine/Data/struct_voxelMap.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"


//Constructor / Destructor
SLAM_localMap::SLAM_localMap(){
  //---------------------------

  this->map = new voxelMap();
  this->map_cloud = new slamMap();

  this->voxel_size_localMap = 1;
  this->voxel_size_slamMap = 0.5;

  this->min_root_distance = 5.0f;
  this->max_root_distance = 100.0f;
  this->max_voxel_distance = 150.0f;
  this->map_max_voxelNbPoints = 20;
  this->min_voxel_distance = 0.05;
  this->grid_voxel_size = 1;
  this->max_total_point = 20000;

  this->slamMap_voxelized = false;

  //---------------------------
}
SLAM_localMap::~SLAM_localMap(){}

//Main function
void SLAM_localMap::reset(){
  //---------------------------

  delete map;
  delete map_cloud;

  this->map = new voxelMap();
  this->map_cloud = new slamMap();

  //---------------------------
}

void SLAM_localMap::compute_gridSampling(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Subsample the scan with voxels
  gridMap grid;
  Eigen::Vector4f point;
  for (int i=0; i<subset->xyz.size(); i++){
    vec3& xyz = subset->xyz[i];

    int kx = static_cast<int>(xyz.x / grid_voxel_size);
    int ky = static_cast<int>(xyz.y / grid_voxel_size);
    int kz = static_cast<int>(xyz.z / grid_voxel_size);
    int key = (kx*200 + ky)*100 + kz;

    point << xyz.x, xyz.y, xyz.z, subset->ts_n[i];
    grid[key].push_back(point);
  }

  //Clear vectors
  frame->xyz.clear();
  frame->xyz_raw.clear();
  frame->ts_n.clear();

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++) {
    if (it->second.size() > 0) {
      //Take one random point
      int rdm = rand() % it->second.size();

      Eigen::Vector4f point = it->second[rdm];
      Eigen::Vector3f xyz(point(0), point(1), point(2));

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

      int kx = static_cast<int>(xyz.x / voxel_size_slamMap);
      int ky = static_cast<int>(xyz.y / voxel_size_slamMap);
      int kz = static_cast<int>(xyz.z / voxel_size_slamMap);
      int key = (kx*200 + ky)*100 + kz;

      //if the voxel already exists
      if(map_cloud->find(key) != map_cloud->end()){
        //Get corresponding voxel
        vector<vec3>& voxel_xyz = map_cloud->find(key).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_max_voxelNbPoints){
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
    Eigen::Vector3f point = frame->xyz[i];
    float dist = fct_distance_origin(point);

    if(dist > min_root_distance && dist < max_root_distance){
      int kx = static_cast<int>(point(0) / voxel_size_localMap);
      int ky = static_cast<int>(point(1) / voxel_size_localMap);
      int kz = static_cast<int>(point(2) / voxel_size_localMap);
      int key = (kx*200 + ky)*100 + kz;

      //if the voxel already exists
      if(map->find(key) != map->end()){
        //Get corresponding voxel
        vector<Eigen::Vector3f>& voxel_xyz = map->find(key).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_max_voxelNbPoints){

          //Check if minimal distance with voxel points is respected
          float dist_min = 10000;
          for(int j=0; j<voxel_xyz.size(); j++){
            Eigen::Vector3f& voxel_point = voxel_xyz[j];
            float dist = fct_distance(point, voxel_point);
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
        vector<Eigen::Vector3f> vec;
        vec.push_back(point);
        map->insert({key, vec});
      }
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
void SLAM_localMap::end_clearTooFarVoxels(Eigen::Vector3f &current_location){
  vector<int> voxels_to_erase;
  //---------------------------

  for(auto it = map->begin(); it != map->end(); ++it){
    Eigen::Vector3f voxel_point = it->second[0];
    float dist = fct_distance(voxel_point, current_location);

    if(dist > max_voxel_distance){
      voxels_to_erase.push_back(it->first);
    }

  }

  for(int i=0; i<voxels_to_erase.size(); i++){
    map->erase(voxels_to_erase[i]);
  }

  //---------------------------
}
