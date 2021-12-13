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

  //---------------------------
}
