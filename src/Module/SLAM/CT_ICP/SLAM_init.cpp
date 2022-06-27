#include "SLAM_init.h"

#include "SLAM_map.h"

#include "../Slam.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"


//Constructor / Destructor
SLAM_init::SLAM_init(Slam* slam){
  //---------------------------

  Engine_node* node_engine = slam->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->mapManager = slam->get_slam_map();

  //---------------------------
}
SLAM_init::~SLAM_init(){}

//Main functions
void SLAM_init::compute_initialization(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  Frame* frame_m2 = sceneManager->get_frame_byID(cloud, subset_ID-2);
  //---------------------------

  this->init_frame_ID(cloud, subset_ID);
  this->init_frame_ts(subset);
  this->init_frame_chain(frame, frame_m1, frame_m2);

  //---------------------------
}

//Subfunctions
void SLAM_init::init_frame_ID(Cloud* cloud, int subset_ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  slamap* slam_map = mapManager->get_slam_map();
  //---------------------------

  //Assign the last local map ID
  frame->ID = slam_map->current_frame_ID;
  slam_map->current_frame_ID++;

  //Assign the current cloud to the selected one
  slam_map->linked_cloud_ID = cloud->ID;

  //---------------------------
}
void SLAM_init::init_frame_ts(Subset* subset){
  Frame* frame = &subset->frame;
  vector<float>& ts = subset->ts;
  //---------------------------

  //Clear vector
  subset->ts_n.clear();

  //If there is timestamp data, normalize it
  if(ts.size() != 0){
    //Retrieve min & max
    double min = ts[0];
    double max = ts[0];
    for(int i=0; i<ts.size(); i++){
      if(ts[i] > max) max = ts[i];
      if(ts[i] < min) min = ts[i];
    }

    //Normalization
    for(int i=0; i<ts.size(); i++){
      double ts_n = (ts[i] - min) / (max - min);
      subset->ts_n.push_back(ts_n);
    }
  }
  //If there is no timestamp data, create synthetic one
  else{
    for(int i=0; i<subset->xyz.size(); i++){
      double ts_n = i / subset->xyz.size();
      subset->ts_n.push_back(ts_n);
    }
  }

  //---------------------------
}
void SLAM_init::init_frame_chain(Frame* frame_m0, Frame* frame_m1, Frame* frame_m2){
  //---------------------------

  //For the first 2 reference frames
  if(frame_m0->ID < 2){
    frame_m0->rotat_b = Eigen::Matrix3d::Identity();
    frame_m0->rotat_e = Eigen::Matrix3d::Identity();
    frame_m0->trans_b = Eigen::Vector3d::Zero();
    frame_m0->trans_e = Eigen::Vector3d::Zero();
  }
  //Other frame
  else{
    Eigen::Matrix3d rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3d trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame_m0->rotat_b = frame_m1->rotat_e;
    frame_m0->trans_b = frame_m1->trans_e;
    frame_m0->rotat_e = rotat_next_e;
    frame_m0->trans_e = trans_next_e;
  }

  //---------------------------
}
