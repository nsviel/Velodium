#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Network/MQTT/MQTT.h"
#include "../../Interface/Network/MQTT/PAHO.h"
#include "../../Interface/Network/HTTP/HTTP.h"
#include "../../Specific/color.h"

#include "imgui/imgui_stdlib.h"


//Constructor / Destructor
GUI_Network::GUI_Network(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->mqttManager = node_interface->get_mqttManager();
  this->httpsManager = node_interface->get_httpsManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Network::~GUI_Network(){}

void GUI_Network::design_Network(){
  //---------------------------

  if(ImGui::BeginTabItem("MQTT")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "MQTT connection");
    this->mqtt_connection();
    this->mqtt_parameter();

    ImGui::EndTabItem();
  }

  if(ImGui::BeginTabItem("HTTP")){
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "HTTP connection");
    this->http_connection();
    this->http_parameter();

    ImGui::EndTabItem();
  }

  //---------------------------
}

//MQTT
void GUI_Network::mqtt_connection(){
  PAHO* mqtt_sncf = mqttManager->get_mqtt_sncf();
  //---------------------------

  ImGui::Text("SNCF broker connected ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", mqtt_sncf->get_is_connected() ? "ON" : "OFF");
  ImGui::Separator();

  //Connect to MQTT broker
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Test", ImVec2(item_width, 0))){
    mqtt_sncf->paho_test_localhost();
  }
  ImGui::Separator();

  //Connect to MQTT broker
  if(ImGui::Button("Connection", ImVec2(item_width, 0))){
    mqtt_sncf->paho_connection();
  }

  //Connect to MQTT broker
  string* message = mqtt_sncf->get_message();
  if(ImGui::Button("Send message", ImVec2(item_width, 0))){
    mqtt_sncf->paho_publish(*message);
  }

  //Connect to MQTT broker
  if(ImGui::Button("Disonnect", ImVec2(item_width, 0))){
    mqtt_sncf->paho_disconnect();
  }
  ImGui::PopStyleColor(1);

  //---------------------------
  ImGui::Separator();
}
void GUI_Network::mqtt_parameter(){
  PAHO* mqtt_sncf = mqttManager->get_mqtt_sncf();
  //---------------------------

  //Text mesage
  string* message = mqtt_sncf->get_message();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Message", message);

  //Destinataire

  //Connection port
  int* mqtt_port = mqtt_sncf->get_selected_port();
  ImGui::SetNextItemWidth(item_width + 20);
  if(ImGui::InputInt("Port", mqtt_port)){
    mqtt_sncf->paho_build_address();
  }

  // Topic
  string* topic = mqtt_sncf->get_topic();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Topic", topic);

  //Client ID
  string* client_ID = mqtt_sncf->get_client_ID();
  ImGui::SetNextItemWidth(item_width + 20);
  ImGui::InputText("Client ID", client_ID);

  //---------------------------
  ImGui::Separator();
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
