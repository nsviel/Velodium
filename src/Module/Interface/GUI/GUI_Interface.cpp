#include "GUI_Interface.h"

#include "GUI_Lidar.h"

#include "../Interface_node.h"
#include "../Network/Network.h"
#include "../Network/MQTT/MQTT.h"
#include "../Network/MQTT/Alert.h"
#include "../Network/SFTP/SFTP.h"
#include "../Network/SFTP/SSH.h"

#include "../../Module_node.h"
#include "../../Module_GUI.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Specific/color.h"

#include "imgui/imgui_stdlib.h"


//Constructor / Destructor
GUI_Interface::GUI_Interface(GUI_module* node_gui){
  //---------------------------

  Module_node* node_module = node_gui->get_node_module();
  Engine_node* node_engine = node_gui->get_node_engine();
  Interface_node* node_interface = node_module->get_node_interface();

  this->sceneManager = node_engine->get_sceneManager();
  this->netManager = node_interface->get_netManager();
  this->mqttManager = netManager->get_mqttManager();
  this->alertManager = netManager->get_alertManager();
  this->sftpManager = netManager->get_sftpManager();
  this->sshManager = netManager->get_sshManager();
  this->gui_lidarManager = new GUI_Lidar(node_gui);

  this->item_width = 100;

  //---------------------------
}
GUI_Interface::~GUI_Interface(){}

void GUI_Interface::design_Interface(){
  //---------------------------

  if(ImGui::BeginTabBar("Interface", ImGuiTabBarFlags_None)){
    //-------------------------------

    // Offline cloud player
    if(ImGui::BeginTabItem("Network")){
      this->design_Network();
      ImGui::EndTabItem();
    }

    // LiDAR management
    if(ImGui::BeginTabItem("LiDAR")){
      gui_lidarManager->design_Velodyne();
      gui_lidarManager->design_Scala();
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  //---------------------------
}
void GUI_Interface::design_Network(){
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SSH connexion");
  this->ssh_connection();
  this->ssh_parameter();

  ImGui::Separator();

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connexion");
  this->mqtt_connection();
  this->mqtt_parameter();

  ImGui::Separator();

  //---------------------------
}

void GUI_Interface::ssh_connection(){
  //---------------------------

  int* adress_ID = sshManager->get_ssh_adress_ID();
  ImGui::Combo("IP", adress_ID, "localHost\0Ordi Louis\0");

  bool is_connected = netManager->get_is_connected();
  if(is_connected == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start SSH", ImVec2(item_width, 0))){
      netManager->start_connection();
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(175, 0, 0, 255));
    if(ImGui::Button("Stop SSH", ImVec2(item_width, 0))){
      netManager->stop_connection();
    }
    ImGui::PopStyleColor(1);
  }
  ImGui::SameLine();

  //Send file
  if(ImGui::Button("Send file", ImVec2(item_width, 0))){
    string path_source = *netManager->get_path_source();
    string path_target = *netManager->get_path_target();
    netManager->send_file(path_source, path_target);
  }

  //---------------------------
}
void GUI_Interface::ssh_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Source path
    string path_source = *netManager->get_path_source();
    if(ImGui::Button("Source", ImVec2(40,0))){
      netManager->select_sourcePath();
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_source.c_str());

    //Target path
    string path_target = *netManager->get_path_target();
    if(ImGui::Button("Target", ImVec2(40,0))){
      netManager->select_targetPath();
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_target.c_str());

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Interface::mqtt_connection(){
  //---------------------------

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Test MQTT", ImVec2(item_width, 0))){
    mqttManager->mqtt_test_localhost();
  }
  ImGui::PopStyleColor(1);

  //Text mesage
  string* message = mqttManager->get_message();
  ImGui::InputText("Message", message);

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Test Alert", ImVec2(item_width, 0))){
    Subset* subset = sceneManager->get_subset_selected();
    alertManager->send_prediction_by_mqtt(subset);
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Interface::mqtt_parameter(){
  if(ImGui::CollapsingHeader("Parameters##2")){
    //---------------------------

    // Topic
    string* topic = mqttManager->get_topic();
    ImGui::InputText("Topic", topic);

    //Client ID
    string* client_ID = mqttManager->get_client_ID();
    ImGui::InputText("Client ID", client_ID);

    //Server adress
    string* server_address = mqttManager->get_broker_address();
    ImGui::InputText("Server adress", server_address);

    //---------------------------
    ImGui::Separator();
  }
}
