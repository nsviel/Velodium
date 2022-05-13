#include "GUI_Interface.h"

#include "GUI_Lidar.h"
#include "GUI_Network.h"

#include "../Interface_node.h"
#include "../Interface.h"
#include "../Local/Saving.h"
#include "../Network/GPS.h"
#include "../Network/HTTP/HTTP_server.h"
#include "../Local/Prediction.h"

#include "../../Module_node.h"
#include "../../Module_GUI.h"


//Constructor / Destructor
GUI_Interface::GUI_Interface(GUI_module* gui_module){
  //---------------------------

  Module_node* node_module = gui_module->get_node_module();
  Interface_node* node_interface = node_module->get_node_interface();

  this->gui_lidarManager = new GUI_Lidar(gui_module);
  this->gui_netManager = new GUI_Network(gui_module);
  this->interfaceManager = node_interface->get_interfaceManager();
  this->saveManager = node_interface->get_saveManager();
  this->predManager = node_interface->get_predManager();
  this->gpsManager = node_interface->get_gpsManager();
  this->httpManager = node_interface->get_httpManager();

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

  if(ImGui::BeginTabItem("MQTT")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connection");
    gui_netManager->mqtt_connection();
    gui_netManager->mqtt_parameter();

    ImGui::EndTabItem();
  }

  if(ImGui::BeginTabItem("HTTP")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "HTTP connection");
    gui_netManager->http_connection();
    gui_netManager->http_parameter();

    ImGui::EndTabItem();
  }

  if(ImGui::BeginTabItem("SSH / SFTP")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SSH");
    gui_netManager->ssh_connection();
    gui_netManager->ssh_parameter();

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SFTP");
    gui_netManager->sftp_connection();

    ImGui::EndTabItem();
  }

  //---------------------------
}

void GUI_Interface::parameter_dynamic(){
  //---------------------------

  //Save frame in folder for AI module
  bool* with_save_frame = interfaceManager->get_with_save_frame();
  ImGui::Checkbox("Save frame", with_save_frame);
  if(*with_save_frame){
    int* save_frame_max = saveManager->get_save_frame_max();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Nb frame", save_frame_max);
  }

  //Save image for interfacing
  bool* with_save_image = interfaceManager->get_with_save_image();
  ImGui::Checkbox("Save image", with_save_image);
  if(*with_save_image){
    int* save_image_max = saveManager->get_save_image_max();

    static bool save_image_unique = false;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    if(ImGui::Checkbox("Unique", &save_image_unique)){
      *save_image_max = 1;
    }

    if(save_image_unique == false){
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb image", save_image_max);
    }
  }

  //---------------------------
}
void GUI_Interface::state_watcher(){
  //---------------------------

  //Runtime AI
  bool with_prediction = *predManager->get_with_prediction();
  ImGui::Text("Runtime - AI");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_prediction ? "ON" : "OFF");

  //Runtime GPS
  bool with_gps = *gpsManager->get_with_gps();
  ImGui::Text("Runtime - GPS");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_gps ? "ON" : "OFF");

  //HTTP server daemon
  bool with_daemon = httpManager->get_is_daemon();
  ImGui::Text("Daemon - HTTP");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_daemon ? "ON" : "OFF");

  //watchers lidar
  gui_lidarManager->state_watcher();

  //---------------------------
}
void GUI_Interface::state_dynamic(){
  //---------------------------

  bool with_save_frame = *interfaceManager->get_with_save_frame();
  ImGui::Text("Online - Save frame");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_frame ? "ON" : "OFF");

  bool with_save_image = *interfaceManager->get_with_save_image();
  ImGui::Text("Online - Save image");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_image ? "ON" : "OFF");

  //---------------------------
}
