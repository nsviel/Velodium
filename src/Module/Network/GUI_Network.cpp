#include "GUI_Network.h"

#include "Network.h"


//Constructor / Destructor
GUI_Network::GUI_Network(){
  //---------------------------

  this->netManager = new Network();

  //---------------------------
}
GUI_Network::~GUI_Network(){}

void GUI_Network::design_Network(){
  //---------------------------

  int* adress_ID = netManager->get_ssh_adress_ID();
  ImGui::Combo("IP", adress_ID, "localHost\0Ordi Louis\0");

  bool* ssh_connected = netManager->get_ssh_connected();
  if(*ssh_connected == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Connexion", ImVec2(150,0))){
      netManager->ssh_connexion();
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(175, 0, 0, 255));
    if(ImGui::Button("Deconnexion", ImVec2(150,0))){
      netManager->ssh_stopConnexion();
    }
    ImGui::PopStyleColor(1);
  }

  //Send file
  if(ImGui::Button("Send file", ImVec2(150,0))){
    netManager->sftp_sendFile();
  }
  ImGui::SameLine();
  if(ImGui::Button("...")){
    netManager->select_fileToSend();
  }

  //display source and target path
  string path_source = *netManager->get_path_source();
  ImGui::Text("Source:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_source.c_str());
  string path_target = *netManager->get_path_target();
  ImGui::Text("Target:");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", path_target.c_str());

  //---------------------------
}
