#include "GUI_Online.h"
#include "GUI_Player.h"

#include "../Node_gui.h"
#include "../Control/GUI_Color.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Online.h"
#include "../../Operation/Dynamic/Player.h"
#include "../../Operation/Dynamic/Saving.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Camera/Followup.h"
#include "../../Engine/OpenGL/Camera/Renderer.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Module/Node_module.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Online::GUI_Online(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();

  this->node_engine = node_gui->get_node_engine();
  this->node_module = node_engine->get_node_module();

  this->filterManager = node_ope->get_filterManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->followManager = node_engine->get_followManager();
  this->savingManager = node_ope->get_savingManager();
  this->renderManager = node_engine->get_renderManager();
  this->colorManager = node_ope->get_colorManager();
  this->configManager = node_engine->get_configManager();
  this->playerManager = node_ope->get_playerManager();

  this->gui_color = node_gui->get_gui_color();
  this->gui_player = node_gui->get_gui_player();

  this->item_width = 100;

  //---------------------------
}
GUI_Online::~GUI_Online(){}

//Main function
void GUI_Online::design_dynamic(){
  if(ImGui::BeginTabItem("Dynamic")){
    if(ImGui::BeginTabBar("##tabs_dynamic", ImGuiTabBarFlags_None)){
      //---------------------------

      gui_player->design_player();
      this->design_online();
      this->design_state();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Online::design_state(){
  if(ImGui::BeginTabItem("State")){
    //---------------------------

    this->state_configuration();
    this->state_online();
    this->state_time();

    //---------------------------
    ImGui::EndTabItem();
  }
}
void GUI_Online::design_online(){
  if(ImGui::BeginTabItem("Online")){
    //---------------------------

    this->parameter_online();
    this->parameter_export();
    gui_color->colorization_choice();

    //---------------------------
    ImGui::EndTabItem();
  }
}

//Parameter function
void GUI_Online::parameter_online(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Module online stuff
  node_module->draw_online();

  //Camera auto displacement
  bool* with_camera_follow = followManager->get_with_camera_follow();
  ImGui::Checkbox("Camera follow up", with_camera_follow);

  //Camera follow absolute position
  if(*with_camera_follow){
    bool* with_camera_absolute = followManager->get_with_camera_absolute();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width);
    ImGui::Checkbox("Absolute positionning", with_camera_absolute);
  }

  //Cylinder cleaning filter
  bool* cylinderFilter = onlineManager->get_with_cylinder_filter();
  ImGui::Checkbox("Cylinder cleaning", cylinderFilter);
  if(*cylinderFilter){
    float* r_min = filterManager->get_cyl_r_min();
    float* r_max = filterManager->get_cyl_r_max();
    float* z_min = filterManager->get_cyl_z_min();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("z min", z_min, 0.1f, 1.0f, "%.2f");
  }

  //---------------------------
}
void GUI_Online::parameter_export(){
  //---------------------------

  //Save frame in folder for AI module
  bool* with_save_frame = savingManager->get_with_save_frame();
  ImGui::Checkbox("Save frame", with_save_frame);
  if(*with_save_frame){
    int* save_frame_max = savingManager->get_save_frame_max();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Nb frame", save_frame_max);
  }

  //Save image for interfacing
  bool* with_save_image = savingManager->get_with_save_image();
  ImGui::Checkbox("Save image", with_save_image);
  if(*with_save_image){
    int* save_image_max = savingManager->get_save_image_max();

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
  }

  //---------------------------
}

//State function
void GUI_Online::state_time(){
  Subset* subset = sceneManager->get_subset_selected();
  Frame* frame = &subset->frame;
  //---------------------------

  float time_operation = onlineManager->get_time_operation();
  ImGui::Text("Operation");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_operation);

  int time_slam = 0;
  if(sceneManager->get_is_list_empty() == false){
    time_slam = (int)frame->time_slam;
  }
  ImGui::Text("SLAM");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", time_slam);

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

  bool with_camera_follow = *followManager->get_with_camera_follow();
  ImGui::Text("Online - Camera follow");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_camera_follow ? "ON" : "OFF");

  string color_name = colorManager->get_color_mode_name();
  ImGui::Text("Online - Colorization");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", color_name.c_str());

  bool with_save_frame = *savingManager->get_with_save_frame();
  ImGui::Text("Online - Save frame");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_frame ? "ON" : "OFF");

  bool with_save_image = *savingManager->get_with_save_image();
  ImGui::Text("Online - Save image");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_image ? "ON" : "OFF");

  //---------------------------
  ImGui::Separator();
}
