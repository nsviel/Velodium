#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Network/HTTP/HTTP.h"
#include "../../Interface/IO/PCAP/PCAP_reader.h"
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

  //Display list of device
  ImGui::Text("Device list");
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 255));
  vector<string> devices_name = pcapManager->get_devices_name();
  string* device_selected = pcapManager->get_device_selected();
  static int selected = 0; // Here we store our selection data as an index.
  if(ImGui::BeginListBox("##434", ImVec2(-FLT_MIN, (devices_name.size()+0.25) * ImGui::GetTextLineHeightWithSpacing()) )){
    for(int n=0; n<devices_name.size(); n++){
      bool is_selected;
      if(devices_name[n] == *device_selected){
        is_selected = true;
      }else{
        is_selected = false;
      }

      //Selection stuff
      string device_name = to_string(n) + ".   " + devices_name[n];
      if(ImGui::Selectable(device_name.c_str(), is_selected)){
        *device_selected = devices_name[n];
      }
      if(is_selected){
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndListBox();
  }
  ImGui::PopStyleColor();

  // PCAP capture options
  bool* with_snif_and_save = pcapManager->get_snif_and_save();
  ImGui::Checkbox("Save packet to file", with_snif_and_save);

  bool* make_ope = pcapManager->get_snif_and_save();
  ImGui::Checkbox("Make online ope", make_ope);


  //pcapManager->retrieve_device();
  //pcapManager->snif_and_save_pcap();


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
