#include "SLAM_init.h"

#include "SLAM_map.h"

#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"


//Constructor / Destructor
SLAM_init::SLAM_init(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->slam_map = slam->get_slam_map();

  //---------------------------
}
SLAM_init::~SLAM_init(){}

//Main functions
void SLAM_init::compute_initialization(Cloud* cloud, int subset_ID){
  //---------------------------

  this->init_frame_ID(cloud, subset_ID);
  this->init_frame_ts(cloud, subset_ID);
  this->init_frame_chain(cloud, subset_ID);

  //---------------------------
}

//Subfunctions
void SLAM_init::init_frame_ID(Cloud* cloud, int subset_ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  slamap* local_map = slam_map->get_local_map();
  //---------------------------

  //Assign the last local map ID
  frame->ID = local_map->current_frame_ID;
  local_map->current_frame_ID++;

  //Assign the current cloud to the selected one
  local_map->linked_cloud_ID = cloud->ID;

  //---------------------------
}
void SLAM_init::init_frame_ts(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = &subset->frame;
  vector<float>& ts = subset->ts;
  //---------------------------

  //Clear vector
  subset->ts_n.clear();

  //If there is timestamp data, normalize it
  if(ts.size() != 0){
    //Retrieve min & max
    double ts_min = ts[0];
    double ts_max = ts[0];
    for(int i=0; i<ts.size(); i++){
      if(ts[i] > ts_max) ts_max = ts[i];
      if(ts[i] < ts_min) ts_min = ts[i];
    }

    //Normalization
    for(int i=0; i<ts.size(); i++){
      double ts_n = (ts[i] - ts_min) / (ts_max - ts_min);
      subset->ts_n.push_back(ts_n);
    }
  }
  //If there is no timestamp data, create synthetic one
  else{
    console.AddLog("error" ,"[SLAM] No timestamp");
    for(int i=0; i<subset->xyz.size(); i++){
      double ts_n = i / subset->xyz.size();
      subset->ts_n.push_back(ts_n);
    }
  }

  //---------------------------
}
void SLAM_init::init_frame_chain(Cloud* cloud, int subset_ID){
  Frame* frame_m0 = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  Frame* frame_m2 = sceneManager->get_frame_byID(cloud, subset_ID-2);
  slamap* local_map = slam_map->get_local_map();
  //---------------------------

  //For the first 2 reference frames
  if(frame_m0->ID < 2){
    frame_m0->trans_b = Eigen::Vector3d::Zero();
    frame_m0->trans_e = Eigen::Vector3d::Zero();
    frame_m0->rotat_b = Eigen::Matrix3d::Identity();
    frame_m0->rotat_e = Eigen::Matrix3d::Identity();
  }
  //Other frame
  else{
    Eigen::Matrix3d rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3d trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    //Which one is better ? I dont know
    frame_m0->rotat_b = frame_m1->rotat_e;
    frame_m0->trans_b = frame_m1->trans_e;
    //frame_m0->rotat_b = rotat_next_b;
    //frame_m0->trans_b = trans_next_b;

    frame_m0->rotat_e = rotat_next_e;
    frame_m0->trans_e = trans_next_e;

    Eigen::Vector3d t_diff = frame_m0->trans_e - frame_m0->trans_b;
    if (t_diff.norm() > 5.0) {
      std::cout << "Error in ego-motion distance !" << std::endl;
    }
  }

  //---------------------------
}
