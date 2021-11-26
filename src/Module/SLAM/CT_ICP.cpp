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

  ceresManager = new SLAM_optim_ceres();
  gnManager = new SLAM_optim_gn();
  normalManager = new SLAM_normal();

  this->solver_ceres = true;
  this->solver_GN = false;
  this->sampling_size = 0.1;
  this->size_voxelMap = 0.5f;
  this->voxel_sizeMax = 20;

  //---------------------------
}
CT_ICP::~CT_ICP(){}

void CT_ICP::compute_slam(){
  Cloud* cloud = database.cloud_selected;
  if(cloud == nullptr) return;
  //---------------------------

  say("---");
  //truc();

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];
    Frame* frame = &cloud->subset[i].frame;
    Frame* frame_m1 = &cloud->subset[i-1].frame;
    Frame* frame_m2 = &cloud->subset[i-2].frame;
    frame->ID = i;

    this->init_frameTimestamp(subset);
    this->init_frameChain(frame, frame_m1, frame_m2);

    this->compute_gridSampling(subset);
    this->compute_optimization(frame, frame_m1);

    this->add_keypointsToCloud(subset);
    this->add_pointsToLocalMap(frame);

    if(i==5)break;
  }

  //---------------------------
}

void CT_ICP::init_frameTimestamp(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  if(frame->ts_n.size() == 0){

    //Timestamp
    vector<float>& ts = subset->ts;
    if(ts.size() != 0 || frame->ID > 1){

      //Retrieve min & max
      double min = ts[0];
      double max = ts[0];
      for(int i=0; i<ts.size(); i++){
        if(ts[i] > max) max = ts[i];
        if(ts[i] < min) min = ts[i];
      }
      frame->ts_b = min;
      frame->ts_e = max;

      //Normalization
      for(int i=0; i<ts.size(); i++){
        double ts_n = (ts[i] - min) / (max - min);
        frame->ts_n.push_back(ts_n);
      }

    }else{
      frame->ts_n = fct_ones(subset->xyz.size());
      frame->ts_b = 1.0f;
      frame->ts_e = 1.0f;
    }

  }

  //---------------------------
}
void CT_ICP::init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2){
  //Si je comprend bien, met juste des matrices identité aux transformation de chaque frame.....
  //---------------------------

  //i == 0 is the reference frame
  if(frame->ID <= 1){
    frame->rotat_b = Eigen::Matrix3d::Identity();
    frame->rotat_e = Eigen::Matrix3d::Identity();
    frame->trans_b = Eigen::Vector3d::Zero();
    frame->trans_e = Eigen::Vector3d::Zero();
  }
  //Second frame
  else if(frame->ID == 2){
    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;

    // Different regimen for the second frame due to the bootstrapped elasticity
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }
  //Other frame
  else if(frame->ID >= 3){
    // When continuous: use the previous begin_pose as reference
    Eigen::Matrix3d rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3d trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);

    // What (?)
    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;

    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }

  //---------------------------
}

void CT_ICP::compute_gridSampling(Subset* subset){
  //---------------------------

  vector<vec3>& subset_xyz = subset->xyz;
  vector<Eigen::Vector3d>& frame_xyz = subset->frame.xyz;
  vector<Eigen::Vector3d>& frame_raw = subset->frame.xyz_raw;

  //Subsample the scan with voxels
  std::map<string, std::vector<glm::vec3>> grid;
  for (int j=0; j<subset_xyz.size(); j++){

    int kx = static_cast<int>(subset_xyz[j].x / sampling_size);
    int ky = static_cast<int>(subset_xyz[j].y / sampling_size);
    int kz = static_cast<int>(subset_xyz[j].z / sampling_size);

    string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);
    grid[voxel_id].push_back(subset_xyz[j]);
  }

  //Take one random point inside each voxel
  frame_xyz.clear();
  frame_raw.clear();
  for (const auto &n: grid) {
    if (n.second.size() > 0) {
      Eigen::Vector3d point = glm_to_eigen_vec3_d(n.second[0]);
      frame_xyz.push_back(point);
      frame_raw.push_back(point);
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

  if(frame->ID >= 2){
    if(solver_GN){
      gnManager->optim_GN();
    }else if(solver_ceres){
      ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}

void CT_ICP::add_keypointsToCloud(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  /*for(int i=0; i<subset->xyz.size(); i++){
    Eigen::Vector3d point = glm_to_eigen_vec3_d(subset->xyz[i]);
    point = frame->rotat_e * point + frame->trans_e;
    subset->xyz[i] = eigen_to_glm_vec3_d(point);
  }*/

  //Transform
  /*for(int i=0; i<frame->xyz.size(); i++){
    frame->xyz[i] = frame->rotat_e * frame->xyz_raw[i] + frame->trans_e;
  }*/

  //Display keypoints
  vector<vec3> keypoint = eigen_to_glm_vectorvec3_d(frame->xyz);
sayHello();say(frame->ID);say(keypoint.size());
subset->xyz.clear();
subset->RGB.clear();
  for(int i=0; i<keypoint.size(); i++){
    subset->xyz.push_back(keypoint[i]);
    subset->RGB.push_back(vec4(1.0f,0.0f,0.0f,1.0f));
  }

  //---------------------------
}
void CT_ICP::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];

    int vx = static_cast<int>(point(0) / size_voxelMap);
    int vy = static_cast<int>(point(1) / size_voxelMap);
    int vz = static_cast<int>(point(2) / size_voxelMap);

    //Search for pre-existing voxel in local map
    string voxel_id = to_string(vx) + " " + to_string(vy) + " " + to_string(vz);
    auto search = map.find(voxel_id);

    //if the voxel already exists
    if (search != map.end()) {
      //Get corresponding voxel
      vector<Eigen::Vector3d>& voxel_xyz = search->second;

      //If the voxel is not full
      if (voxel_xyz.size() < voxel_sizeMax){
        voxel_xyz.push_back(point);
      }
    }
    //else create it
    else{
      map[voxel_id].push_back(point);
    }

  }

  //---------------------------
}
