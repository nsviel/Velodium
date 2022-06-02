#include "GUI_Obstacle.h"

#include "GUI_Dynamic.h"

#include "../Player_node.h"
#include "../Player_GUI.h"
#include "../Obstacle/Obstacle.h"
#include "../Dynamic/Online.h"

#include "../../Interface/LiDAR/Capture.h"
#include "../../Module_GUI.h"
#include "../../Module_node.h"
#include "../../SLAM/GUI/GUI_Slam.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Functions/CoordTransform.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(GUI_module* node){
  this->gui_module = node;
  //---------------------------

  Operation_node* node_ope = gui_module->get_node_ope();
  Module_node* node_module = gui_module->get_node_module();
  Engine_node* node_engine = gui_module->get_node_engine();
  Player_node* node_player = node_module->get_node_player();

  this->coordManager = node_ope->get_coordManager();
  this->obstacleManager = node_player->get_obstacleManager();
  this->sceneManager = node_engine->get_sceneManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

//Main function
void GUI_Obstacle::design_obstacle(){
  //---------------------------

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

//Subfunctions
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
void GUI_Obstacle::parameter_online(){
  //---------------------------

  //SLAM parameters
  GUI_Slam* gui_slam = gui_module->get_gui_slam();
  gui_slam->parameter_lidar();

  //With MQTT warning
  bool* with_warning = obstacleManager->get_with_warning();
  ImGui::Checkbox("With MQTT prediction", with_warning);

  //Online parameters
  GUI_Player* gui_player = gui_module->get_gui_player();
  GUI_Dynamic* gui_dynamic = gui_player->get_gui_dynamic();
  gui_dynamic->parameter_online_options();

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
}
