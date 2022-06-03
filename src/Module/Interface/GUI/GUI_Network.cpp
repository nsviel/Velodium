#include "GUI_Network.h"

#include "../Interface_node.h"
#include "../Network/Network.h"
#include "../Network/struct_wallet.h"
#include "../Network/MQTT/MQTT.h"
#include "../Network/SFTP/SFTP.h"
#include "../Network/SFTP/SSH.h"
#include "../Network/HTTP/HTTPS.h"

#include "../../Module_node.h"
#include "../../Module_GUI.h"

#include "../../../Operation/Color/color.h"

#include "imgui/imgui_stdlib.h"


//Constructor / Destructor
GUI_Network::GUI_Network(GUI_module* gui_module){
  //---------------------------

  Module_node* node_module = gui_module->get_node_module();
  Interface_node* node_interface = node_module->get_node_interface();
  Network* netManager = node_interface->get_netManager();

  this->netManager = node_interface->get_netManager();
  this->mqttManager = netManager->get_mqttManager();
  this->sftpManager = netManager->get_sftpManager();
  this->sshManager = netManager->get_sshManager();
  this->httpsManager = netManager->get_httpsManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Network::~GUI_Network(){}

//MQTT
void GUI_Network::mqtt_connection(){
  //---------------------------

  ImGui::Text("Broker connected ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", mqttManager->get_is_connected() ? "ON" : "OFF");
  ImGui::Separator();

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Test", ImVec2(item_width, 0))){
    mqttManager->mqtt_test_localhost();
  }
  ImGui::Separator();

  //Connect to MQTT broker
  if(ImGui::Button("Connection", ImVec2(item_width, 0))){
    mqttManager->mqtt_connection();
  }

  //Connect to MQTT broker
  string* message = mqttManager->get_message();
  if(ImGui::Button("Send message", ImVec2(item_width, 0))){
    mqttManager->mqtt_send_message(*message);
  }

  //Connect to MQTT broker
  if(ImGui::Button("Disonnect", ImVec2(item_width, 0))){
    mqttManager->mqtt_disconnect();
  }
  ImGui::PopStyleColor(1);

  //---------------------------
  ImGui::Separator();
}
void GUI_Network::mqtt_parameter(){
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

//HTTP
void GUI_Network::http_connection(){
  //---------------------------

  ImGui::Text("Daemon activated ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", httpsManager->get_is_https_deamon() ? "ON" : "OFF");
  ImGui::Separator();

  //---------------------------
}
void GUI_Network::http_parameter(){
  //---------------------------

  //---------------------------
}

//SSH
void GUI_Network::ssh_connection(){
  Wallet* wallet = netManager->get_wallet();
  //---------------------------

  //Connection button
  bool is_connected = netManager->get_is_connected();
  if(is_connected == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Connection SSH", ImVec2(item_width, 0))){
      netManager->start_connection();
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(175, 0, 0, 255));
    if(ImGui::Button("Disconnect SSH", ImVec2(item_width, 0))){
      netManager->stop_connection();
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Network::ssh_parameter(){
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

  //Connection port
  int* ssh_port = sshManager->get_selected_port();
  ImGui::SetNextItemWidth(item_width + 20);
  if(ImGui::InputInt("Port", ssh_port)){
    sshManager->ssh_disconnection();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Network::sftp_connection(){
  //---------------------------

  //Send file
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Send file", ImVec2(item_width, 0))){
    netManager->send_file();
  }
  ImGui::PopStyleColor(1);

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
}
