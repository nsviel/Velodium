#include "GUI_Obstacle.h"

#include "GUI_Dynamic.h"
#include "GUI_Player.h"

#include "../Player_node.h"
#include "../Obstacle/Obstacle.h"
#include "../Dynamic/Online.h"

#include "../../Interface/Interface_node.h"
#include "../../Interface/Local/Prediction.h"
#include "../../Interface/Local/GPS.h"
#include "../../Interface/Network/Network.h"
#include "../../Interface/GUI/GUI_Lidar.h"
#include "../../Interface/LiDAR/Capture.h"
#include "../../Module_GUI.h"
#include "../../Module_node.h"
#include "../../SLAM/GUI/GUI_Slam.h"

#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../GUI/GUI_node.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Color/Color.h"
#include "../../../Operation/Functions/CoordTransform.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(GUI_module* node){
  this->node_gui = node;
  //---------------------------

  Module_node* node_module = node_gui->get_node_module();
  Operation_node* node_ope = node_gui->get_node_ope();
  Engine_node* node_engine = node_gui->get_node_engine();

  this->node_player = node_module->get_node_player();
  this->coordManager = node_ope->get_coordManager();
  this->node_interface = node_module->get_node_interface();
  this->obstacleManager = node_player->get_obstacleManager();
  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->colorManager = node_ope->get_colorManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

//Main function
void GUI_Obstacle::design_Obstacle(){
  //---------------------------

  this->state_configuration();
  this->state_watcher();
  this->state_online();
  this->parameter_online();
  this->parameter_interfacing();

  //---------------------------
}
void GUI_Obstacle::runtime_display_naming(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  if(cloud != nullptr){
    //---------------------------

    Subset* subset = cloud->subset_selected;
    Obstac* obstacle_pr = &subset->obstacle_pr;

    this->label_ID = 0;
    for(int j=0; j<obstacle_pr->name.size(); j++){
      string name = obstacle_pr->name[j];
      vec3 position = obstacle_pr->position[j];
      position.z = obstacle_pr->dimension[j].z;

      this->compute_draw_text(name, position);

      this->label_ID++;
    }

    //---------------------------
  }
}

//Actions
void GUI_Obstacle::state_configuration(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Configuration");
  //---------------------------

  //Choose configuration
  int config_selected = *configManager->get_config();
  if(ImGui::Combo("##007", &config_selected, "Default\0AI module\0WP4 car\0WP5 train\0")){
    configManager->make_preconfig(config_selected);
    node_player->update();
  }

  //Start scenario
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Start##1", ImVec2(item_width, 0))){
    //Start watcher here
    //create maybe a new class which manage watchers
  }
  ImGui::PopStyleColor(1);

  //---------------------------
  ImGui::Separator();
}
void GUI_Obstacle::compute_draw_text(string text, vec3 position){
  //---------------------------

  //Overlay flags
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  window_flags |= ImGuiWindowFlags_NoMove;

  //Convert world position to screen location
  vec2 uv = coordManager->WorldToScreen(position);

  //Obstacle labeling
  if(uv.x != -1 && uv.y != -1){
    bool truc = true;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));
    ImGui::SetNextWindowBgAlpha(0.75f);
    ImGui::SetNextWindowPos(ImVec2(uv.x, uv.y));

    string window_name = "label_" + to_string(this->label_ID);
    ImGui::Begin(window_name.c_str(), &truc, window_flags);

    //Supress window borders
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;

    //Supress potential focus
    if(ImGui::IsWindowHovered()){
      ImGuiIO& io = ImGui::GetIO();
      io.WantCaptureMouse = false;
    }

    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "%s", text.c_str());

    ImGui::End();
    ImGui::PopStyleColor();
  }

  //---------------------------
}

//State
void GUI_Obstacle::state_watcher(){
  //---------------------------

  //Watchers
  Module_node* node_module = node_gui->get_node_module();
  Interface_node* node_interface = node_module->get_node_interface();
  Capture* captureManager = node_interface->get_captureManager();
  Prediction* predManager = node_interface->get_predManager();
  GPS* gpsManager = node_interface->get_gpsManager();

  bool is_velodyne_watcher = captureManager->get_is_capture_watcher();
  ImGui::Text("Watcher - Velodyne");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_velodyne_watcher ? "ON" : "OFF");

  bool is_scala_watcher = captureManager->get_is_capture_watcher();
  ImGui::Text("Watcher - Scala");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_scala_watcher ? "ON" : "OFF");

  bool with_prediction = *predManager->get_with_prediction();
  ImGui::Text("Runtime - AI");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_prediction ? "ON" : "OFF");

  bool with_gps = *gpsManager->get_with_gps();
  ImGui::Text("Runtime - GPS");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_gps ? "ON" : "OFF");

  //---------------------------
  ImGui::Separator();
}
void GUI_Obstacle::state_online(){
  //---------------------------

  //Specific module
  Module_node* node_module = node_gui->get_node_module();
  Player_node* node_player = node_module->get_node_player();
  Online* onlineManager = node_player->get_onlineManager();

  bool with_slam = *onlineManager->get_with_slam();
  ImGui::Text("Online - SLAM");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_slam ? "ON" : "OFF");

  bool with_camera_follow = *onlineManager->get_with_camera_follow();
  ImGui::Text("Online - Camera follow");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_camera_follow ? "ON" : "OFF");

  bool with_save_frame = *onlineManager->get_with_save_frame();
  ImGui::Text("Online - Save frame");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_frame ? "ON" : "OFF");

  bool with_save_image = *onlineManager->get_with_save_image();
  ImGui::Text("Online - Save image");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_image ? "ON" : "OFF");

  string color_name = colorManager->get_color_mode_name();
  ImGui::Text("Online - Colorization");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", color_name.c_str());

  //---------------------------
  ImGui::Separator();
}

//Parameters
void GUI_Obstacle::parameter_online(){
  //---------------------------

  //Online parameters
  GUI_Player* gui_player = node_gui->get_gui_player();
  GUI_Dynamic* gui_dynamic = gui_player->get_gui_dynamic();
  gui_dynamic->parameter_online();

  //SLAM parameters
  GUI_Slam* gui_slam = node_gui->get_gui_slam();
  gui_slam->parameter_slam();

  //---------------------------
}
void GUI_Obstacle::parameter_interfacing(){
  //Obstacle detection parameters
  if(ImGui::CollapsingHeader("Parameter - interface")){
    //---------------------------

    //Prediction directory
    if(ImGui::Button("...##1")){
      //node_interface->select_dir_path();
    }
    ImGui::SameLine();
    string dir_path = node_interface->get_dir_path();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

    //Add predictions
    if(ImGui::Button("Add predictions")){
      obstacleManager->add_obstacle_pred();
    }

    //Add ground truth
    if(ImGui::Button("Add ground truth")){
      obstacleManager->add_obstacle_grTr();
    }

    //---------------------------
  }
}
