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

  if(ImGui::Button("Load##1", ImVec2(150,0))){
    scalaManager->loading();
  }

  //---------------------------
}
