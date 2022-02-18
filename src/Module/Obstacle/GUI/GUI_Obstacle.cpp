#include "GUI_Obstacle.h"

#include "../Obstacle.h"
#include "../Scenario.h"
#include "../Interface/Interfacing.h"
#include "../Interface/Prediction.h"
#include "../Interface/GPS.h"

#include "../../Module_GUI.h"
#include "../../Module_node.h"
#include "../../LiDAR/GUI/GUI_Lidar.h"
#include "../../LiDAR/Capture.h"
#include "../../Player/GUI/GUI_Player.h"
#include "../../Network/Network.h"
#include "../../SLAM/GUI/GUI_Slam.h"

#include "../../../Engine/Scene.h"
#include "../../../GUI/GUI_node.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Functions/CoordTransform.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(GUI_node* node){
  this->node_gui = node;
  //---------------------------

  Module_node* node_module = node_gui->get_node_module();
  Operation_node* node_ope = node_gui->get_node_ope();

  this->coordManager = node_ope->get_coordManager();
  this->obstacleManager = node_module->get_obstacleManager();
  this->ioManager = node_module->get_ioManager();
  this->scenarioManager = node_module->get_scenarioManager();
  this->sceneManager = new Scene();

  this->item_width = 100;

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

//Main function
void GUI_Obstacle::design_Obstacle(){
  //---------------------------

  this->compute_scenario();
  this->watcher_state();
  this->online_state();
  this->watcher_activation();
  this->parameter();

  //---------------------------
}

//Subfunctions
void GUI_Obstacle::compute_scenario(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Scenario");
  //---------------------------

  //Choose scenario
  int* scenario_selected = scenarioManager->get_scenario_selected();
  ImGui::Combo("##007", scenario_selected, "WP4 auto\0WP5 train on-board\0WP5 train on-edge\0");

  //Start scenario
  bool scenario_started = *scenarioManager->get_scenario_started();
  if(scenario_started == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start scenario", ImVec2(item_width, 0))){
      scenarioManager->scenario_start();
    }
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(133, 45, 45, 255));
    if(ImGui::Button("Stop scenario", ImVec2(item_width, 0))){
      scenarioManager->scenario_stop();
    }
  }
  ImGui::PopStyleColor(1);


  //---------------------------
  ImGui::Separator();
}
void GUI_Obstacle::watcher_activation(){
  if(ImGui::CollapsingHeader("Watcher")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    //---------------------------

    //AI watcher
    this->watcher_AI_pred();

    //GPS watcher
    this->watcher_gps();

    //Capture watcher
    GUI_module* gui_module = node_gui->get_gui_moduleManager();
    GUI_Lidar* gui_lidarManager = gui_module->get_gui_lidarManager();
    gui_lidarManager->velo_capture();

    //MQTT messager
    //GUI_Network* gui_netManager = gui_module->get_gui_netManager();
    //gui_netManager->mqtt_connection();

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Obstacle::watcher_state(){
  //---------------------------

  //Watchers
  Module_node* node_module = node_gui->get_node_module();
  Capture* captureManager = node_module->get_captureManager();
  Interfacing* ioManager = node_module->get_ioManager();
  Prediction* predManager = ioManager->get_predManager();
  GPS* gpsManager = ioManager->get_gpsManager();

  bool is_capture_watcher = captureManager->get_is_capture_watcher();
  ImGui::Text("Watcher - Capture");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_capture_watcher ? "ON" : "OFF");

  bool is_pred_watcher = predManager->get_is_watching();
  ImGui::Text("Watcher - AI");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_pred_watcher ? "ON" : "OFF");

  bool is_gps_watcher = gpsManager->get_is_watching();
  ImGui::Text("Watcher - GPS");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_gps_watcher ? "ON" : "OFF");

  //---------------------------
  ImGui::Separator();
}
void GUI_Obstacle::online_state(){
  //---------------------------

  //Specific module
  Module_node* node_module = node_gui->get_node_module();
  Player_online* onlineManager = node_module->get_onlineManager();

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

  //---------------------------
  ImGui::Separator();
}
void GUI_Obstacle::parameter(){
  //---------------------------

  //Interfacing parameters
  this->parameter_interfacing();

  //Online parameters
  GUI_module* gui_module = node_gui->get_gui_moduleManager();
  GUI_Player* gui_playerManager = gui_module->get_gui_playerManager();
  gui_playerManager->parameter_online();

  //SLAM parameters
  GUI_Slam* gui_slamManager = gui_module->get_gui_slamManager();
  gui_slamManager->parameter_slam();

  //---------------------------
}
void GUI_Obstacle::parameter_interfacing(){
  //Obstacle detection parameters
  if(ImGui::CollapsingHeader("Interfacing params")){
    //---------------------------

    //Prediction directory
    if(ImGui::Button("...##1")){
      ioManager->select_dir_path();
    }
    ImGui::SameLine();
    string dir_path = ioManager->get_dir_path();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

    //---------------------------
  }
}

//Subfunctions
void GUI_Obstacle::watcher_AI_pred(){
  //---------------------------

  //IA watchers
  Prediction* predManager = ioManager->get_predManager();
  bool is_watching = predManager->get_is_watching();
  if(is_watching == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start AI prediction", ImVec2(item_width,0))){
      predManager->start_watcher_prediction();
    }
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(133, 45, 45, 255));
    if(ImGui::Button("Stop AI prediction", ImVec2(item_width,0))){
      predManager->stop_watcher_prediction();
    }
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Obstacle::watcher_gps(){
  //---------------------------

  //IA watchers
  GPS* gpsManager = ioManager->get_gpsManager();
  bool is_watching = gpsManager->get_is_watching();
  if(is_watching == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start GPS", ImVec2(item_width,0))){
      gpsManager->start_watcher_gps();
    }
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(133, 45, 45, 255));
    if(ImGui::Button("Stop GPS", ImVec2(item_width,0))){
      gpsManager->stop_watcher_gps();
    }
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}

//Obstacle visual naming
void GUI_Obstacle::runtime_prediction_naming(){
  Subset* subset = sceneManager->get_subset_selected();
  if(subset == nullptr) return;
  label_ID = 0;
  //---------------------------

  Obstac* obstacle_pr = &subset->obstacle_pr;

  for(int j=0; j<obstacle_pr->name.size(); j++){
    string name = obstacle_pr->name[j];
    vec3 position = obstacle_pr->position[j];
    position.z = obstacle_pr->dimension[j].z;

    this->drawText(name, position);

    label_ID++;
  }

  //---------------------------
}
void GUI_Obstacle::drawText(string text, vec3 position){
  //---------------------------

  //Overlay flags
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
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

    string window_name = "label_" + to_string(label_ID);
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
