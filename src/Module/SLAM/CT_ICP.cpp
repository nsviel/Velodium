#include "CT_ICP.h"

#include "CT_ICP/SLAM_optim_ceres.h"
#include "CT_ICP/SLAM_optim_gn.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"

#include "../../Engine/Data/Database.h"
#include "../../Engine/Scene.h"

#include <map>
#include <ceres/ceres.h>

extern struct Database database;

#include "CT_ICP/truc.h"

//Constructor / Destructor
CT_ICP::CT_ICP(){
  //---------------------------

  sceneManager = new Scene();
  ceresManager = new SLAM_optim_ceres();
  gnManager = new SLAM_optim_gn();
  normalManager = new SLAM_normal();

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = false;
  this->sampling_width = 1;
  this->map_voxel_width = 1;
  this->map_max_voxelNbPoints = 20;
  this->frame_max = 0;
  this->frame_all = true;
  this->slamMap_voxelized = false;

  //---------------------------
}
CT_ICP::~CT_ICP(){}

void CT_ICP::compute_slam(){
  Cloud* cloud = database.cloud_selected;
  map = new voxelMap();
  if(cloud == nullptr) return;
  if(frame_all) frame_max = cloud->nb_subset;
  //---------------------------

  for(int i=0; i<frame_max; i++){
    Subset* subset = &cloud->subset[i];
    Frame* frame = &cloud->subset[i].frame;
    Frame* frame_m1 = &cloud->subset[i-1].frame;
    Frame* frame_m2 = &cloud->subset[i-2].frame;
    frame->ID = i;

    tic();

    this->init_frameTimestamp(subset);
    this->init_frameChain(frame, frame_m1, frame_m2);
    this->init_distortion(frame);

    this->compute_gridSampling(subset);
    this->compute_optimization(frame, frame_m1);

    this->add_pointsToSlamMap(subset);
    this->add_pointsToLocalMap(frame);


    float duration = toc();
    if(verbose){
      cout<<"Compute SLAM for " << subset->name.c_str() <<" in "<< duration << " ms" << endl;
    }
  }

  //---------------------------
  delete map;
}

void CT_ICP::init_frameTimestamp(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Timestamp
  vector<float>& ts = subset->ts;
  if(ts.size() != 0 && frame->ID > 1){

    //Retrieve min & max
    double min = ts[0];
    double max = ts[0];
    for(int i=0; i<ts.size(); i++){
      if(ts[i] > max) max = ts[i];
      if(ts[i] < min) min = ts[i];
    }
    subset->ts_b = min;
    subset->ts_e = max;

    //Normalization
    subset->ts_n.clear();
    for(int i=0; i<ts.size(); i++){
      double ts_n = (ts[i] - min) / (max - min);
      subset->ts_n.push_back(ts_n);
    }

  }else{
    subset->ts_n = fct_ones(subset->xyz.size());
    subset->ts_b = 1.0f;
    subset->ts_e = 1.0f;
  }

  //---------------------------
}
void CT_ICP::init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2){
  //---------------------------

  //i == 0 is the reference frame
  if(frame->ID < 2){
    frame->rotat_b = Eigen::Matrix3d::Identity();
    frame->rotat_e = Eigen::Matrix3d::Identity();
    frame->trans_b = Eigen::Vector3d::Zero();
    frame->trans_e = Eigen::Vector3d::Zero();
  }
  //Second frame
  else if(frame->ID == 2){
    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;

    // Different for the second frame due to the bootstrapped elasticity
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }
  //Other frame
  else if(frame->ID > 2){
    // When continuous: use the previous begin_pose as reference
    Eigen::Matrix3d rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3d trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);

    // What (?)
    //frame->rotat_b = rotat_next_b;
    //frame->trans_b = trans_next_b;
    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;

    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }

  //---------------------------
}
void CT_ICP::init_distortion(Frame* frame){
  //---------------------------

  if(frame->ID > 1){
    gnManager->frame_distort(frame);
  }

  //---------------------------
}

void CT_ICP::compute_gridSampling(Subset* subset){
  //---------------------------

  vector<Eigen::Vector3d>& frame_xyz = subset->frame.xyz;
  vector<Eigen::Vector3d>& frame_raw = subset->frame.xyz_raw;
  vector<float>& frame_ts_n = subset->frame.ts_n;

  //Subsample the scan with voxels
  std::map<string, std::vector<glm::vec4>> grid;
  for (int j=0; j<subset->xyz.size(); j++){
    vec3 xyz = subset->xyz[j];
    float ts_n = subset->ts_n[j];

    int kx = static_cast<int>(xyz.x / sampling_width);
    int ky = static_cast<int>(xyz.y / sampling_width);
    int kz = static_cast<int>(xyz.z / sampling_width);

    string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);
    vec4 point = vec4(xyz.x, xyz.y, xyz.z, ts_n);
    grid[voxel_id].push_back(point);
  }

  //Take one random point inside each voxel
  frame_xyz.clear();
  frame_raw.clear();
  frame_ts_n.clear();

  int cpt =0;
  for (const auto &n: grid) {
    if (n.second.size() > 0) {
      cpt++;

      vec4 point = n.second[0];
      vec3 xyz = vec3(point.x, point.y, point.z);
      float ts_n = point[3];

      Eigen::Vector3d xyz_eig = glm_to_eigen_vec3_d(xyz);
      frame_xyz.push_back(xyz_eig);
      frame_raw.push_back(xyz_eig);
      frame_ts_n.push_back(ts_n);
    }
  }

  //---------------------------
}
void CT_ICP::compute_normal(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  if(frame->ID > 1){
    normalManager->compute_frameNormal(frame, map);
  }

  //---------------------------
}
void CT_ICP::compute_optimization(Frame* frame, Frame* frame_m1){
  //---------------------------

  if(frame->ID >= 1){
    if(solver_GN){
      gnManager->optim_GN(frame, frame_m1, map);
    }else if(solver_ceres){
      ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}

void CT_ICP::add_pointsToSlamMap(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  //Update frame root
  Eigen::Vector3d root = glm_to_eigen_vec3_d(subset->root);
  Eigen::Matrix3d R = quat_b.toRotationMatrix();
  Eigen::Vector3d t = trans_b;
  root = R * root + t;
  subset->root = eigen_to_glm_vec3_d(root);

  //Update subset position
  for(int i=0; i<subset->xyz.size(); i++){
    Eigen::Vector3d point = glm_to_eigen_vec3_d(subset->xyz[i]);
    float ts_n = subset->ts_n[i];

    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;
    point = R * point + t;

    if(slamMap_voxelized){
      //TODO: voxelized SLAM map
    }else{
      subset->xyz[i] = eigen_to_glm_vec3_d(point);
    }

  }

  //---------------------------
  sceneManager->update_subset_location(subset);
  sceneManager->update_subset_color(subset);
}
void CT_ICP::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];

    int vx = static_cast<int>(point(0) / map_voxel_width);
    int vy = static_cast<int>(point(1) / map_voxel_width);
    int vz = static_cast<int>(point(2) / map_voxel_width);

    //Search for pre-existing voxel in local map
    string voxel_id = to_string(vx) + " " + to_string(vy) + " " + to_string(vz);

    //if the voxel already exists
    if(map->find(voxel_id) != map->end()){
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = map->find(voxel_id).value();

      //If the voxel is not full
      if (voxel_xyz.size() < map_max_voxelNbPoints){
        voxel_xyz.push_back(point);
      }
    }
    //else create it
    else{
      vector<Eigen::Vector3d> vec;
      vec.push_back(point);
      map->insert({voxel_id, vec});
    }
  }

  //---------------------------
}
