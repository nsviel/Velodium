#include "SLAM_localMap.h"

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

  vector<Eigen::Vector3f>& frame_xyz = frame->xyz;
  vector<Eigen::Vector3f>& frame_raw = frame->xyz_raw;
  vector<float>& frame_ts_n = frame->ts_n;

  //Subsample the scan with voxels
  gridMap grid;
  for (int j=0; j<subset->xyz.size(); j++){
    Eigen::Vector3f xyz = glm_to_eigen_vec3_d(subset->xyz[j]);
    float ts_n = subset->ts_n[j];

    int kx = static_cast<int>(xyz(0) / grid_voxel_size);
    int ky = static_cast<int>(xyz(1) / grid_voxel_size);
    int kz = static_cast<int>(xyz(2) / grid_voxel_size);
    string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);

    Eigen::Vector4d point(xyz(0), xyz(1), xyz(2), ts_n);
    grid[voxel_id].push_back(point);
  }

  //Clear vectors
  frame_xyz.clear();
  frame_raw.clear();
  frame_ts_n.clear();

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); ++it) {
    if (it->second.size() > 0) {
      Eigen::Vector4d point = it->second[0];
      Eigen::Vector3f xyz(point(0), point(1), point(2));

      frame_xyz.push_back(xyz);
      frame_ts_n.push_back(point(3));
    }
  }
  frame_raw = frame_xyz;

  //---------------------------
}
void SLAM_localMap::add_pointsToSlamMap(Subset* subset){
  //---------------------------

  //Insert points into cloud global slam map
  if(slamMap_voxelized){

    for (int i=0; i<subset->xyz.size(); i++){
      vec3 point = subset->xyz[i];

      int kx = static_cast<int>(point.x / voxel_size_slamMap);
      int ky = static_cast<int>(point.y / voxel_size_slamMap);
      int kz = static_cast<int>(point.z / voxel_size_slamMap);
      string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);

      //if the voxel already exists
      if(map_cloud->find(voxel_id) != map_cloud->end()){
        //Get corresponding voxel
        vector<vec3>& voxel_xyz = map_cloud->find(voxel_id).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_max_voxelNbPoints){
          voxel_xyz.push_back(point);
          subset->xyz_voxel.push_back(point);
        }
      }
      //else create it
      else{
        vector<vec3> vec;

        vec.push_back(point);
        subset->xyz_voxel.push_back(point);

        map_cloud->insert({voxel_id, vec});
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
      int vx = static_cast<int>(point(0) / voxel_size_localMap);
      int vy = static_cast<int>(point(1) / voxel_size_localMap);
      int vz = static_cast<int>(point(2) / voxel_size_localMap);

      //Search for pre-existing voxel in local map
      string voxel_id = to_string(vx) + " " + to_string(vy) + " " + to_string(vz);

      //if the voxel already exists
      if(map->find(voxel_id) != map->end()){
        //Get corresponding voxel
        vector<Eigen::Vector3f>& voxel_xyz = map->find(voxel_id).value();

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
        map->insert({voxel_id, vec});
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
  vector<string> voxels_to_erase;
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
