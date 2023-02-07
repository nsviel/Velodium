#include "GUI_Capture.h"
#include "GUI_Network.h"
#include "GUI_Scala.h"
#include "GUI_Velodyne.h"
#include "GUI_Recorder.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Scala/Scala.h"
#include "../../Interface/Capture/Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Capture::GUI_Capture(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->gui_network = node_gui->get_gui_network();
  this->gui_scala = new GUI_Scala(node_gui);
  this->gui_velodyne = new GUI_Velodyne(node_gui);
  this->gui_recorder = new GUI_Recorder(node_gui);

  this->item_width = 100;

  //---------------------------
}
GUI_Capture::~GUI_Capture(){}

//Main function
void GUI_Capture::design_interface(){
  if(ImGui::BeginTabItem("Interface")){
    if(ImGui::BeginTabBar("##Interface", ImGuiTabBarFlags_None)){
      //---------------------------

      this->design_capture();
      gui_network->design_Network();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Capture::design_capture(){
  if(ImGui::BeginTabItem("Capture")){
    if(ImGui::BeginTabBar("##tabs_capture", ImGuiTabBarFlags_None)){
      //---------------------------

      if(ImGui::BeginTabItem("Velodyne")){
        gui_velodyne->design_Velodyne();
        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("Scala")){
        gui_scala->design_Scala();
        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("Recorder")){
        this->state_watcher();
        gui_recorder->design_recorder();
        ImGui::EndTabItem();
      }

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}

//Specific functions
void GUI_Capture::state_watcher(){
  //---------------------------

  gui_velodyne->velo_state();
  gui_scala->scala_state();

  //---------------------------
  ImGui::Separator();
}
