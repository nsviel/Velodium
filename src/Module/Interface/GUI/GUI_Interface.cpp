#include "GUI_Interface.h"

#include "GUI_Lidar.h"

#include "../Interface_node.h"
#include "../Network/Network.h"
#include "../Network/struct_wallet.h"
#include "../Network/MQTT/MQTT.h"
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

    // LiDAR management
    if(ImGui::BeginTabItem("LiDAR")){
      gui_lidarManager->design_Velodyne();
      gui_lidarManager->design_Scala();
      ImGui::EndTabItem();
    }

    // Offline cloud player
    if(ImGui::BeginTabItem("SSH")){
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SSH connexion");
      this->ssh_connection();
      this->ssh_parameter();

      ImGui::EndTabItem();
    }

    // MQTT management
    if(ImGui::BeginTabItem("MQTT")){
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connexion");
      this->mqtt_connection();
      this->mqtt_parameter();

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  //---------------------------
}

void GUI_Interface::ssh_connection(){
  Wallet* wallet = netManager->get_wallet();
  //---------------------------

  //Destinataire
  string* selected_ip = sshManager->get_selected_ip();
  string* selected_dest = sshManager->get_selected_dest();
  ImGui::SetNextItemWidth(item_width + 20);
  if (ImGui::BeginCombo((*selected_ip).c_str(), (*selected_dest).c_str())){
    for (int i=0; i<wallet->wallet_name.size(); i++){
      string name = wallet->wallet_name[i];
      if(ImGui::Selectable(name.c_str())){
        *selected_dest = name;
        *selected_ip = wallet->wallet_dic.find(name)->second;
      }
    }
    ImGui::EndCombo();
  }

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
  //---------------------------

  //Connection port
  int* ssh_port = sshManager->get_selected_port();
  ImGui::SetNextItemWidth(item_width + 20);
  if(ImGui::InputInt("Port", ssh_port)){
    sshManager->ssh_disconnection();
  }

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
void GUI_Interface::mqtt_connection(){
  //---------------------------

  ImGui::Text("Broker connected ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", mqttManager->get_is_connected() ? "ON" : "OFF");

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Full test", ImVec2(item_width, 0))){
    mqttManager->mqtt_test_localhost();
  }
  ImGui::SameLine();

  //Connect to MQTT broker
  string* message = mqttManager->get_message();
  if(ImGui::Button("Continuous test", ImVec2(item_width, 0))){
    mqttManager->mqtt_send_message(*message);
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Interface::mqtt_parameter(){
  Wallet* wallet = netManager->get_wallet();
  //---------------------------

  //Text mesage
  string* message = mqttManager->get_message();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Message", message);

  //Destinataire
  string* selected_ip = mqttManager->get_selected_ip();
  string* selected_dest = mqttManager->get_selected_dest();
  ImGui::SetNextItemWidth(item_width + 20);
  if (ImGui::BeginCombo((*selected_ip).c_str(), (*selected_dest).c_str())){
    for (int i=0; i<wallet->wallet_name.size(); i++){
      string name = wallet->wallet_name[i];
      if(ImGui::Selectable(name.c_str())){
        *selected_dest = name;
        *selected_ip = wallet->wallet_dic.find(name)->second;
        mqttManager->mqtt_build_address();
      }
    }
    ImGui::EndCombo();
  }

  //Connection port
  int* mqtt_port = mqttManager->get_selected_port();
  ImGui::SetNextItemWidth(item_width + 20);
  if(ImGui::InputInt("Port", mqtt_port)){
    mqttManager->mqtt_build_address();
  }

  // Topic
  string* topic = mqttManager->get_topic();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Topic", topic);

  //Client ID
  string* client_ID = mqttManager->get_client_ID();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Client ID", client_ID);

  //---------------------------
  ImGui::Separator();
}
