#include "GUI_Interface.h"

#include "GUI_Lidar.h"
#include "GUI_Network.h"

#include "../../Module_node.h"
#include "../../Module_GUI.h"


//Constructor / Destructor
GUI_Interface::GUI_Interface(GUI_module* node_gui){
  //---------------------------

  this->gui_lidarManager = new GUI_Lidar(node_gui);
  this->gui_netManager = new GUI_Network(node_gui);

  this->item_width = 100;

  //---------------------------
}
GUI_Interface::~GUI_Interface(){}

void GUI_Interface::design_Interface(){
  //---------------------------

  if(ImGui::BeginTabBar("Interface", ImGuiTabBarFlags_None)){
    //-------------------------------

    // LiDAR management
    this->design_Lidar();

    // Network management
    this->design_Network();

    ImGui::EndTabBar();
  }

  //---------------------------
}
void GUI_Interface::design_Lidar(){
  //---------------------------

  if(ImGui::BeginTabItem("LiDAR")){
    gui_lidarManager->design_Velodyne();
    gui_lidarManager->design_Scala();
    ImGui::EndTabItem();
  }

  //---------------------------
}
void GUI_Interface::design_Network(){
  //---------------------------

  if(ImGui::BeginTabItem("SSH / SFTP")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SSH");
    gui_netManager->ssh_connection();
    gui_netManager->ssh_parameter();

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SFTP");
    gui_netManager->sftp_connection();

    ImGui::EndTabItem();
  }

  if(ImGui::BeginTabItem("MQTT")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connexion");
    gui_netManager->mqtt_connection();
    gui_netManager->mqtt_parameter();

    ImGui::EndTabItem();
  }
  
  //---------------------------
}
