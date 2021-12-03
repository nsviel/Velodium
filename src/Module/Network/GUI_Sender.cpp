#include "GUI_Sender.h"

#include "Sender.h"


//Constructor / Destructor
GUI_Sender::GUI_Sender(){
  //---------------------------

  this->senderManager = new Sender();

  //---------------------------
}
GUI_Sender::~GUI_Sender(){}

void GUI_Sender::design_Sender(){
  //---------------------------

  if(ImGui::Button("Send", ImVec2(150,0))){
    senderManager->send_something();
  }

  //---------------------------
}
