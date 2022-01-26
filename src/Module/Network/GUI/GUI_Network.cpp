#include "GUI_Network.h"

#include "../Network.h"
#include "../Protocol/MQTT/MQTT.h"
#include "../Protocol/SFTP.h"
#include "../Protocol/SSH.h"

#include "../../../../extern/imgui/imgui_stdlib.h"


//Constructor / Destructor
GUI_Network::GUI_Network(){
  //---------------------------

  this->netManager = new Network();
  this->mqttManager = netManager->get_mqttManager();
  this->sftpManager = netManager->get_sftpManager();
  this->sshManager = netManager->get_sshManager();

  //---------------------------
}
GUI_Network::~GUI_Network(){}

void GUI_Network::design_Network(){
  //---------------------------

  this->ssh_connection();
  this->mqtt_connection();
  this->mqtt_parameter();

  //---------------------------
}

void GUI_Network::ssh_connection(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "SSH connexion");
  //---------------------------

  int* adress_ID = sshManager->get_ssh_adress_ID();
  ImGui::Combo("IP", adress_ID, "localHost\0Ordi Louis\0");

  bool* ssh_connected = sshManager->get_ssh_connected();
  if(*ssh_connected == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Connexion", ImVec2(150,0))){
      sshManager->ssh_startConnexion();
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(175, 0, 0, 255));
    if(ImGui::Button("Deconnexion", ImVec2(150,0))){
      sshManager->ssh_stopConnexion();
    }
    ImGui::PopStyleColor(1);
  }

  //Send file
  if(ImGui::Button("Send file", ImVec2(150,0))){
    netManager->send_file();
  }

  //display source and target path
  string path_source = *netManager->get_path_source();
  if(ImGui::Button("Source", ImVec2(40,0))){
    netManager->select_sourcePath();
  }
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_source.c_str());
  string path_target = *netManager->get_path_target();
  if(ImGui::Button("Target", ImVec2(40,0))){
    netManager->select_targetPath();
  }
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_target.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_Network::mqtt_connection(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connexion");
  //---------------------------

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Connection", ImVec2(150,0))){
    mqttManager->mqtt_sendMessages();
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Network::mqtt_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Test localhost connexion
    if(ImGui::Button("Test localhost##1", ImVec2(150,0))){
      mqttManager->mqtt_test_localhost();
    }

    // Topic
    string* topic = mqttManager->get_topic();
    ImGui::InputText("Topic", topic);

    //Text mesage
    string* message = mqttManager->get_message();
    ImGui::InputText("Message", message);

    //Client ID
    string* client_ID = mqttManager->get_client_ID();
    ImGui::InputText("Client ID", client_ID);

    //Server adress
    string* server_address = mqttManager->get_server_address();
    ImGui::InputText("Server adress", server_address);

    //---------------------------
    ImGui::Separator();
  }
}
