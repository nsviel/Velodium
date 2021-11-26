#include "GUI_slam.h"

#include "../CT_ICP.h"
#include "../CT_ICP/SLAM_optim_ceres.h"
#include "../CT_ICP/SLAM_optim_gn.h"

#include "../../../Engine/Data/Database.h"
#include "../../../Engine/Scene.h"

extern struct Database database;


//Constructor / Destructor
GUI_slam::GUI_slam(){
  //---------------------------

  this->cticpManager = new CT_ICP();
  this->slam_optiManager = cticpManager->get_SLAM_optim_ceres();

  //---------------------------
}
GUI_slam::~GUI_slam(){}

//Main function
void GUI_slam::design_SLAM(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  if(ImGui::Button("Compute", ImVec2(75,0))){
    cticpManager->compute_slam();

    Scene sceneManager;
    sceneManager.update_cloud_location(cloud);
  }

  float* sampling_size = cticpManager->get_sampling_size();
  ImGui::InputFloat("Subsample grid size", sampling_size, 0.001f, 1.0f, "%.3f");
  int* iter_max = slam_optiManager->get_iter_max();
  ImGui::SliderInt("Number iter", iter_max, 1, 20);

  //---------------------------
}
