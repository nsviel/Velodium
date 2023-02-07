#include "GUI_Scala.h"
#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"
#include "../../Interface/Capture/Scala/Scala.h"
#include "../../Interface/Capture/Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Scala::GUI_Scala(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->captureManager = node_interface->get_captureManager();
  this->scalaManager = captureManager->get_scalaManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Scala::~GUI_Scala(){}

//Main function
void GUI_Scala::design_Scala(){
  //---------------------------

  this->scala_statistique();
  this->scala_capture();
  this->scala_file();
  this->scala_parameter();

  //---------------------------
  ImGui::Separator();
}

//Scala subfunctions
void GUI_Scala::scala_state(){
  //---------------------------

  //Watcher Scala
  bool is_scala_watcher = *scalaManager->get_run_capture();
  ImGui::Text("Watcher - Scala");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_scala_watcher ? "ON" : "OFF");

  //---------------------------
}
void GUI_Scala::scala_statistique(){
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Scala");

  //---------------------------
}
void GUI_Scala::scala_file(){
  //---------------------------

  if(ImGui::Button("Load in dir", ImVec2(75, 0))){
    //scalaManager->loading("");
  }
  ImGui::SameLine();
  if(ImGui::Button("Load fast", ImVec2(75, 0))){
    //scalaManager->loading("/home/aether/Desktop/Velodium/media/scala");
  }

  //---------------------------
}
void GUI_Scala::scala_capture(){
  bool is_capturing = *scalaManager->get_is_scala_capturing();
  //---------------------------

  //Capturing button
  if(is_capturing == false){
    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start capture", ImVec2(157, 0))){
      captureManager->start_new_capture("scala");
    }
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop capture", ImVec2(157, 0))){
      captureManager->stop_capture();
    }
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Scala::scala_parameter(){
  if(ImGui::CollapsingHeader("Parameters##2")){
    //---------------------------

    //Connection port
    int* scala_port = scalaManager->get_capture_port();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputInt("Port", scala_port);

    //---------------------------
  }
}
