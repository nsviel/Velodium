#include "GUI_Obstacle.h"
#include "GUI_Network.h"

#include "../HTTP/HTTP.h"
#include "../src/Obstacle.h"
#include "../src/Prediction.h"
#include "../Module_obstacle.h"

#include "../../Node_module.h"
#include "../../SLAM/GUI/GUI_Slam.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Function/CoordTransform.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(Module_obstacle* module){
  //---------------------------

  Node_module* node_module = module->get_node_module();
  Node_operation* node_ope = node_module->get_node_ope();
  Node_engine* node_engine = module->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->coordManager = node_ope->get_coordManager();
  this->obstacleManager = module->get_obstacleManager();
  this->predManager = module->get_predManager();
  this->httpsManager = module->get_httpsManager();

  this->gui_network = module->get_gui_network();

  this->item_width = 100;

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

//Main function
void GUI_Obstacle::design_obstacle(){
  if(ImGui::BeginTabItem("Obstacle")){
    if(ImGui::BeginTabBar("##tabs_obstacle", ImGuiTabBarFlags_None)){
      //---------------------------

      this->design_prediction();
      gui_network->design_Network();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Obstacle::design_prediction(){
  if(ImGui::BeginTabItem("Prediction")){
    //---------------------------

    this->state_prediction();
    this->parameter_prediction();

    //---------------------------
    ImGui::EndTabItem();
  }
}

//Draw label text function
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

//Parameter function
void GUI_Obstacle::parameter_prediction(){
  //---------------------------

  //With MQTT warning
  bool* with_warning = obstacleManager->get_with_warning();
  ImGui::Checkbox("With MQTT prediction", with_warning);

  //Prediction directory
  if(ImGui::Button("...##1")){
    //node_interface->select_dir_path();
  }
  ImGui::SameLine();
  //string dir_path = node_interface->get_dir_path();
  //ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

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

//State function
void GUI_Obstacle::state_prediction(){
  //---------------------------

  //Runtime AI
  bool with_prediction = *predManager->get_with_prediction();
  ImGui::Text("Runtime - AI");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_prediction ? "ON" : "OFF");

  //HTTP server daemon
  bool with_daemon = httpsManager->get_is_https_deamon();
  ImGui::Text("Daemon - HTTP");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_daemon ? "ON" : "OFF");

  //---------------------------
}
