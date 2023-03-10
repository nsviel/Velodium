#include "SLAM_map.h"
#include "SLAM_sampling.h"
#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"

#include "../../../Load/Node_load.h"
#include "../../../Load/Processing/Pather.h"

#include "../../../Specific/Function/fct_math.h"
#include "../../../Specific/Function/fct_transtypage.h"


//Constructor / Destructor
SLAM_map::SLAM_map(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->patherManager = node_load->get_patherManager();
  this->slam_sampling = slam->get_slam_sampling();

  this->local_map = new slamap();
  this->local_cloud = new slamap();

  this->with_local_cloud = false;

  //---------------------------
  this->update_configuration();
}
SLAM_map::~SLAM_map(){}

//Main function
void SLAM_map::update_configuration(){
  //---------------------------

  //Local map for SLAM algo
  local_map->reset();
  local_map->voxel_width = 1;
  local_map->voxel_capacity = 20;
  local_map->voxel_min_point_dist = 0.05;
  local_map->voxel_max_dist = 150;

  //Cloud map for voxelized cloud
  local_cloud->reset();
  local_cloud->voxel_width = 0.1;
  local_cloud->voxel_capacity = 200;
  local_cloud->voxel_min_point_dist = 0.05;
  local_cloud->voxel_max_dist = 150;
  local_cloud->dist_min = 3;
  local_cloud->dist_max = 50;

  //---------------------------
}
void SLAM_map::update_map(Collection* collection, int subset_ID){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  Frame* frame = collection->get_frame_byID(subset_ID);
  //---------------------------

  //Local map
  vector<vec3> xyz_map = slam_sampling->sub_sampling_subset(cloud, 0.2);
  this->add_pointToMap(local_map, xyz_map); //frame->xyz
  this->end_clearTooFarVoxels(local_map, frame->trans_e);

  //Cloud map
  if(with_local_cloud){
    this->add_pointToCloud(local_cloud, cloud);
    this->end_clearTooFarVoxels(local_cloud, frame->trans_e);
  }

  //---------------------------
}
void SLAM_map::reset_map(){
  //---------------------------

  local_map->reset();
  local_cloud->reset();

  //---------------------------
}

//sub-function
void SLAM_map::add_pointToMap(slamap* map, vector<vec3>& xyz){
  //---------------------------

  for(int i=0; i<xyz.size(); i++){
    Eigen::Vector3d point;
    point << xyz[i].x, xyz[i].y, xyz[i].z;

    //Retrieve corresponding voxel
    int kx = static_cast<int>(point(0) / map->voxel_width);
    int ky = static_cast<int>(point(1) / map->voxel_width);
    int kz = static_cast<int>(point(2) / map->voxel_width);
    int key = map->get_signature(kx, ky, kz);
    voxelMap_it it = map->map.find(key);

    //if the voxel already exists
    if(it != map->map.end()){
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = it.value();

      //If the voxel is not full
      if(voxel_xyz.size() < map->voxel_capacity){
        //Retrieve the minimal distance between the point and the voxel points
        double dist_min = 10000;
        for(int j=0; j<voxel_xyz.size(); j++){
          Eigen::Vector3d& voxel_point = voxel_xyz[j];
          double dist = fct_distance(point, voxel_point);
          if(dist < dist_min){
            dist_min = dist;
          }
        }

        //If the distance is not too short, add the point to local map
        if(dist_min > map->voxel_min_point_dist){
          voxel_xyz.push_back(point);
        }
      }
    }
    //else create a new voxel
    else{
      vector<Eigen::Vector3d> voxel;
      voxel.push_back(point);
      map->map.insert({key, voxel});
    }
  }

  //---------------------------
}
void SLAM_map::add_pointToMap(slamap* map, Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->xyz.size(); i++){
    Eigen::Vector3d point(cloud->xyz[i].x, cloud->xyz[i].y, cloud->xyz[i].z);

    //Retrieve corresponding voxel
    int kx = static_cast<int>(point(0) / map->voxel_width);
    int ky = static_cast<int>(point(1) / map->voxel_width);
    int kz = static_cast<int>(point(2) / map->voxel_width);
    int key = map->get_signature(kx, ky, kz);
    voxelMap_it it = map->map.find(key);

    //if the voxel already exists
    if(it != map->map.end()){
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = it.value();

      //If the voxel is not full
      if(voxel_xyz.size() < map->voxel_capacity){
        //Retrieve the minimal distance between the point and the voxel points
        double dist_min = 10000;
        for(int j=0; j<voxel_xyz.size(); j++){
          Eigen::Vector3d& voxel_point = voxel_xyz[j];
          double dist = fct_distance(point, voxel_point);
          if(dist < dist_min){
            dist_min = dist;
          }
        }

        //If the distance is not too short, add the point to local map
        if(dist_min > map->voxel_min_point_dist){
          voxel_xyz.push_back(point);
        }
      }
    }
    //else create a new voxel
    else{
      vector<Eigen::Vector3d> voxel;
      voxel.push_back(point);
      map->map.insert({key, voxel});
    }
  }

  //---------------------------
}
void SLAM_map::add_pointToCloud(slamap* map, Cloud* cloud){
  Eigen::Vector3d point_3d;
  Eigen::Vector4d point_4d;
  //---------------------------

  for(int i=0; i<cloud->xyz.size(); i++){
    //Init data format
    float Is = 0;
    if(cloud->I.size() != 0){
      Is = cloud->I[i];
    }
    vec3 xyz = cloud->xyz[i];
    point_3d << xyz.x, xyz.y, xyz.z;
    point_4d << xyz.x, xyz.y, xyz.z, Is;

    //Retrieve corresponding voxel
    int kx = static_cast<int>(xyz.x / map->voxel_width);
    int ky = static_cast<int>(xyz.y / map->voxel_width);
    int kz = static_cast<int>(xyz.z / map->voxel_width);
    int key = map->get_signature(kx, ky, kz);
    cloudMap_it it = map->cloud.find(key);

    //if the voxel already exists
    if(it != map->cloud.end()){
      //Get corresponding voxel
      vector<Eigen::Vector4d>& voxel_xyz = it.value();

      //If the voxel is not full
      if(voxel_xyz.size() < map->voxel_capacity){
        //Retrieve the minimal distance between the point and the voxel points
        double dist_min = 10000;
        for(int j=0; j<voxel_xyz.size(); j++){
          Eigen::Vector3d voxel_point(voxel_xyz[j](0), voxel_xyz[j](1), voxel_xyz[j](2));
          double dist = fct_distance(point_3d, voxel_point);
          if(dist < dist_min){
            dist_min = dist;
          }
        }

        //If the distance is not too short, add the point to local map
        if(dist_min > map->voxel_min_point_dist){
          double dist_origin = fct_distance_origin(point_3d);
          if(dist_origin > map->dist_min && dist_origin < map->dist_max){
            voxel_xyz.push_back(point_4d);
          }
        }
      }
    }
    //else create a new voxel
    else{
      vector<Eigen::Vector4d> voxel;
      voxel.push_back(point_4d);
      map->cloud.insert({key, voxel});
    }
  }

  //---------------------------
}
void SLAM_map::save_local_cloud(){
  Cloud* cloud = new Cloud();
  //---------------------------

  //Put local cloud data into a new cloud
  for(cloudMap_it it = local_cloud->cloud.begin(); it != local_cloud->cloud.end(); it++){
    vector<Eigen::Vector4d>& voxel_xyz = it.value();
    for(int i=0; i<voxel_xyz.size(); i++){
      Eigen::Vector4d& point = voxel_xyz[i];

      vec3 xyz = vec3(point(0), point(1), point(2));
      vec4 rgb = vec4(point(3), point(3), point(3), 1);

      cloud->xyz.push_back(xyz);
      cloud->I.push_back(point(3));
    }
  }

  //Save the new cloud
  patherManager->saving_subset(cloud);

  //---------------------------
}

//Voxel specific functions
void SLAM_map::end_clearTooFarVoxels(slamap* map, Eigen::Vector3d &pose){
  vector<int> voxels_to_erase;
  //---------------------------

  //Retrieve all voxel which are too far from current pose
  for(auto it = map->map.begin(); it != map->map.end(); ++it){
    Eigen::Vector3d voxel_point = it->second[0];
    double dist = fct_distance(voxel_point, pose);

    if(dist > map->voxel_max_dist){
      voxels_to_erase.push_back(it->first);
    }
  }

  //Erase them
  for(int i=0; i<voxels_to_erase.size(); i++){
    map->map.erase(voxels_to_erase[i]);
  }

  //---------------------------
}
