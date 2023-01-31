#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Network/HTTP/HTTP.h"
#include "../../Interface/Network/PCAP/PCAP_reader.h"
#include "../../Specific/color.h"

#include "imgui/imgui_stdlib.h"


//Constructor / Destructor
GUI_Network::GUI_Network(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  //this->mqttManager = node_interface->get_mqttManager();
  this->httpsManager = node_interface->get_httpsManager();
  this->pcapManager = new PCAP_reader();

  this->item_width = 100;

  //---------------------------
}
GUI_Network::~GUI_Network(){}

void GUI_Network::design_Network(){
  if(ImGui::BeginTabItem("Network")){
    if(ImGui::BeginTabBar("##tabs_network", ImGuiTabBarFlags_None)){
      //---------------------------

      if(ImGui::BeginTabItem("HTTP")){
        ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "HTTP connection");
        this->http_connection();
        this->http_parameter();

        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("PCAP")){
        this->pcap_connection();

        ImGui::EndTabItem();
      }

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}

//PCAP stuff
void GUI_Network::pcap_connection(){
  //---------------------------

  pcapManager->retrieve_device();
  pcapManager->snif_and_save_pcap();


  //---------------------------
}

//HTTP
void GUI_Network::http_connection(){
  //---------------------------

  ImGui::Text("Daemon activated: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", httpsManager->get_is_https_deamon() ? "ON" : "OFF");
  ImGui::Text("Port: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d", httpsManager->get_server_port());
  ImGui::Separator();

  //---------------------------
}
void GUI_Network::http_parameter(){
  //---------------------------

  //---------------------------
}
