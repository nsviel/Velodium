#include "SLAM_optim.h"

#include "SLAM_optim_gn.h"

#include "../src/SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Specific/Function/fct_math.h"


//Constructor / Destructor
SLAM_optim::SLAM_optim(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->slam_optim_gn = new SLAM_optim_gn(slam);

  //---------------------------
  this->update_configuration();
}
SLAM_optim::~SLAM_optim(){}

//Main function
void SLAM_optim::update_configuration(){
  //---------------------------

  this->solver_ceres = false;
  this->solver_GN = true;

  //---------------------------
}
void SLAM_optim::compute_optimization(Cloud* cloud, int subset_ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  //---------------------------

  if(frame->ID > 0){
    slam_optim_gn->optim_GN(frame, frame_m1);
  }

  //---------------------------
}
