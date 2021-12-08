#include "GUI_Scala.h"

#include "Scala.h"


//Constructor / Destructor
GUI_Scala::GUI_Scala(){
  //---------------------------

  this->scalaManager = new Scala();

  //---------------------------
}
GUI_Scala::~GUI_Scala(){}

void GUI_Scala::design_Scala(){
  //---------------------------

  if(ImGui::Button("Load in directory", ImVec2(100,0))){
    scalaManager->loading("");
  }

  if(ImGui::Button("Load fast", ImVec2(100,0))){
    scalaManager->loading("/home/aither/Desktop/Velodium/media/scala");
  }

  //---------------------------
}
