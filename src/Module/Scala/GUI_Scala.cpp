#include "GUI_Scala.h"

#include "Scala.h"


//Constructor / Destructor
GUI_scala::GUI_scala(){
  //---------------------------

  this->scalaManager = new Scala();

  //---------------------------
}
GUI_scala::~GUI_scala(){}

void GUI_scala::design_Scala(){
  //---------------------------

  if(ImGui::Button("Load in directory", ImVec2(100,0))){
    scalaManager->loading("");
  }

  if(ImGui::Button("Load fast", ImVec2(100,0))){
    scalaManager->loading("/home/aither/Desktop/Velodium/media/scala");
  }

  //---------------------------
}
