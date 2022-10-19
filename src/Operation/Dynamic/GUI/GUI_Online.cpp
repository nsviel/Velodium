#include "GUI_Online.h"

#include "../Online.h"
#include "../Player.h"
#include "../Saving.h"

#include "../../Node_operation.h"
#include "../../Capture/Capture.h"
#include "../../Capture/LiDAR/Velodyne/Velodyne.h"
#include "../../Color/Color.h"

#include "../../../Engine/OpenGL/Camera/Followup.h"
#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Color/Heatmap.h"
#include "../../../Operation/Transformation/Filter.h"
#include "../../../Operation/Color/GUI/GUI_Color.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Online::GUI_Online(Node_operation* node_ope){
  //---------------------------

  this->node_engine = node_ope->get_node_engine();
  Module_node* node_module = node_engine->get_node_module();

  this->gui_color = node_ope->get_gui_color();
  this->filterManager = node_ope->get_filterManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->followManager = node_engine->get_followManager();
  this->captureManager = node_ope->get_captureManager();
  this->savingManager = node_ope->get_savingManager();
  this->renderManager = node_engine->get_renderManager();
  this->colorManager = node_ope->get_colorManager();
  this->configManager = node_engine->get_configManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Online::~GUI_Online(){}



//Main function
void GUI_Online::design_state(){
  //---------------------------

  this->state_configuration();
  this->state_online();

  //---------------------------
}
void GUI_Online::design_time(){
  Subset* subset = sceneManager->get_subset_selected();
  Frame* frame = &subset->frame;
  //---------------------------

  float time_operation = onlineManager->get_time_operation();
  ImGui::Text("Operation");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_operation);

  bool with_slam = *onlineManager->get_with_slam();
  int time_slam = 0;
  if(sceneManager->get_is_list_empty() == false){
    time_slam = (int)frame->time_slam;
  }
  if(with_slam){
    ImGui::Text("SLAM");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", time_slam);
  }

  bool with_save_frame = *savingManager->get_with_save_frame();
  int time_save_frame = 0;
  if(sceneManager->get_is_list_empty() == false){
    time_save_frame = (int)frame->time_save_frame;
  }
  if(with_save_frame){
    ImGui::Text("Save frame");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", time_save_frame);
  }

  bool with_save_image = *savingManager->get_with_save_frame();
  if(with_save_image){
    float time_screenshot = renderManager->get_time_screenshot();
    ImGui::Text("Save image");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_screenshot);
  }

  //---------------------------
}
void GUI_Online::state_dynamic(){
  //---------------------------

  bool with_save_frame = *savingManager->get_with_save_frame();
  ImGui::Text("Online - Save frame");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_frame ? "ON" : "OFF");

  bool with_save_image = *savingManager->get_with_save_image();
  ImGui::Text("Online - Save image");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_image ? "ON" : "OFF");

  //---------------------------
}
void GUI_Online::parameter_dynamic(){
  //---------------------------
  /*
  //Save frame in folder for AI module
  bool* with_save_frame = savingManager->get_with_save_frame();
  ImGui::Checkbox("Save frame", with_save_frame);
  if(*with_save_frame){
    int* save_frame_max = saveManager->get_save_frame_max();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Nb frame", save_frame_max);
  }

  //Save image for interfacing
  bool* with_save_image = savingManager->get_with_save_image();
  ImGui::Checkbox("Save image", with_save_image);
  if(*with_save_image){
    int* save_image_max = saveManager->get_save_image_max();

    static bool save_image_unique;
    if(*save_image_max == 1){
      save_image_unique = true;
    }else{
      save_image_unique = false;
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    if(ImGui::Checkbox("Unique", &save_image_unique)){
      if(save_image_unique == true){
        *save_image_max = 1;
      }else{
        *save_image_max = 20;
      }
    }

    if(save_image_unique == false){
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb image", save_image_max);
    }
  }*/

  //---------------------------
}
//Main function
/*void GUI_Player::design_player(){
  //---------------------------

  if(ImGui::BeginTabBar("Obstacle##156", ImGuiTabBarFlags_None)){
    //-------------------------------

    // Obstacle detection
    if(ImGui::BeginTabItem("State")){
      gui_online->design_state();
      ImGui::EndTabItem();
    }

    // Obstacle detection
    if(ImGui::BeginTabItem("Online")){
      gui_obstacle->design_obstacle();
      ImGui::EndTabItem();
    }

    // Offline cloud player
    if(ImGui::BeginTabItem("Player")){
      gui_online->design_dynamic();
      ImGui::EndTabItem();
    }

    // Operation time
    if(ImGui::BeginTabItem("Time")){
      gui_online->design_time();
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  //---------------------------
}*/


//Subfunctions
void GUI_Online::state_configuration(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Configuration");
  //---------------------------

  //Choose configuration
  int config_selected = *configManager->get_config();
  if(ImGui::Combo("##007", &config_selected, "Default\0Capture\0AI\0Server\0")){
    configManager->make_preconfig(config_selected);
    node_engine->update();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Online::state_online(){
  //---------------------------

  //Specific module
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

  this->state_dynamic();

  //---------------------------
  ImGui::Separator();
}
