#include "GUI_State.h"

#include "GUI_Dynamic.h"

#include "../Player_GUI.h"
#include "../Player_node.h"
#include "../Dynamic/Online.h"
#include "../Dynamic/Followup.h"

#include "../../Interface/GUI/GUI_Interface.h"
#include "../../Interface/GUI/GUI_Lidar.h"
#include "../../Interface/LiDAR/Capture.h"
#include "../../Interface/Interface.h"
#include "../../Interface/Interface_node.h"
#include "../../Module_GUI.h"
#include "../../Module_node.h"
#include "../../SLAM/GUI/GUI_Slam.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Color/Color.h"


//Constructor / Destructor
GUI_State::GUI_State(GUI_module* gui_module){
  //---------------------------

  Operation_node* node_ope = gui_module->get_node_ope();
  Module_node* node_module = gui_module->get_node_module();
  Engine_node* node_engine = gui_module->get_node_engine();
  Interface_node* node_interface = node_module->get_node_interface();

  this->node_engine = gui_module->get_node_engine();
  this->gui_interface = gui_module->get_gui_interface();
  this->node_player = node_module->get_node_player();
  this->configManager = node_engine->get_configManager();
  this->colorManager = node_ope->get_colorManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->onlineManager = node_player->get_onlineManager();
  this->interfaceManager = node_interface->get_interfaceManager();
  this->renderManager = node_engine->get_renderManager();

  this->item_width = 100;

  //---------------------------
}
GUI_State::~GUI_State(){}

//Main function
void GUI_State::design_state(){
  //---------------------------

  this->state_configuration();
  gui_interface->state_watcher();
  this->state_online();

  //---------------------------
}
void GUI_State::design_time(){
  Subset* subset = sceneManager->get_subset_selected();
  Frame* frame = &subset->frame;
  //---------------------------

  float time_operation = onlineManager->get_time_operation();
  ImGui::Text("Operation");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_operation);

  bool with_slam = *onlineManager->get_with_slam();
  if(with_slam){
    ImGui::Text("SLAM");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)frame->time_slam);
  }

  bool with_save_frame = *interfaceManager->get_with_save_frame();
  if(with_save_frame){
    ImGui::Text("Save frame");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)frame->time_save_frame);
  }

  bool with_save_image = *interfaceManager->get_with_save_frame();
  if(with_save_image){
    float time_screenshot = renderManager->get_time_screenshot();
    ImGui::Text("Save image");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_screenshot);
  }

  //---------------------------
}

//Subfunctions
void GUI_State::state_configuration(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Configuration");
  //---------------------------

  //Choose configuration
  int config_selected = *configManager->get_config();
  if(ImGui::Combo("##007", &config_selected, "Default\0Capture\0AI module\0WP4 car\0WP5 train\0")){
    configManager->make_preconfig(config_selected);
    node_engine->update();
  }

  /*//Start scenario
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Start##1", ImVec2(item_width, 0))){
    //Start watcher here
    //create maybe a new class which manage watchers
  }
  ImGui::PopStyleColor(1);*/

  //---------------------------
  ImGui::Separator();
}
void GUI_State::state_online(){
  //---------------------------

  //Specific module
  Followup* followManager = node_player->get_followManager();

  bool with_slam = *onlineManager->get_with_slam();
  ImGui::Text("Online - SLAM");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_slam ? "ON" : "OFF");

  bool with_camera_follow = *followManager->get_with_camera_follow();
  ImGui::Text("Online - Camera follow");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_camera_follow ? "ON" : "OFF");

  string color_name = colorManager->get_color_mode_name();
  ImGui::Text("Online - Colorization");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", color_name.c_str());

  gui_interface->state_dynamic();

  //---------------------------
  ImGui::Separator();
}
