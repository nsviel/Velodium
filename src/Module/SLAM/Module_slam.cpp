#include "Module_slam.h"

#include "src/SLAM.h"
#include "src/SLAM_parameter.h"
#include "GUI/GUI_Slam.h"
#include "ct_icp/CT_ICP.h"

#include "../Node_module.h"

#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/Node_engine.h"
#include "../../Operation/Node_operation.h"


//Constructor / Destructor
Module_slam::Module_slam(Node_module* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node_engine->get_node_ope();

  this->slamManager = new SLAM(node_engine);
  this->cticpManager = new CT_ICP();
  this->slam_param = slamManager->get_slam_param();
  this->gui_slam = new GUI_Slam(this);

  Configuration* configManager = node_engine->get_configManager();
  this->with_slam = configManager->parse_json_b("module", "with_slam");
  this->algo = 0;

  //---------------------------
}
Module_slam::~Module_slam(){}

void Module_slam::update(){
  //---------------------------

  slamManager->update_configuration();

  //---------------------------
}
void Module_slam::reset(){
  //---------------------------

  slamManager->reset_slam();
  cticpManager->reset();

  //---------------------------
}
void Module_slam::draw(){
  //---------------------------

  gui_slam->design_SLAM();

  //---------------------------
}
void Module_slam::draw_online(){
  //---------------------------

  gui_slam->design_option();

  //---------------------------
}
void Module_slam::online(Cloud* cloud, int subset_ID){
  //---------------------------

  if(with_slam){
    if(algo == 0){
      Subset* subset = cloud->get_subset_byID(subset_ID);
      cticpManager->algo(subset);
    }
    else if(algo == 1){
      slamManager->compute_slam(cloud, subset_ID);
    }
  }

  //---------------------------
}
