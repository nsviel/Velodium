#include "GUI_Obstacle.h"

#include "Obstacle.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(){
  //---------------------------

  this->obstacleManager = new Obstacle();

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

void GUI_Obstacle::design_Obstacle(){
  //---------------------------

  if(ImGui::Button("run", ImVec2(100,0))){
    obstacleManager->run();
  }

  //Prediction directory
  if(ImGui::Button("...##1")){
    obstacleManager->select_dir_path();
  }
  ImGui::SameLine();
  string dir_path = obstacleManager->get_dir_path();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

  //---------------------------
}
