#include "GUI_Obstacle.h"

#include "../Obstacle.h"
#include "../Interfacing.h"

#include "../../../Engine/Scene.h"
#include "../../../GUI/GUI_node.h"
#include "../../../Operation/Functions/CoordTransform.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(GUI_node* node_gui){
  //---------------------------

  Module_node* node_module = node_gui->get_node_module();
  Operation_node* node_ope = node_gui->get_node_ope();

  this->coordManager = node_ope->get_coordManager();
  this->obstacleManager = node_module->get_obstacleManager();
  this->ioManager = node_module->get_ioManager();
  this->sceneManager = new Scene();

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

void GUI_Obstacle::design_Obstacle(){
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  this->compute_obstacle();
  this->make_obstacleName(subset);
  this->parameters();

  //---------------------------
}

void GUI_Obstacle::compute_obstacle(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  bool* is_thread_pred = ioManager->get_is_thread_pred();
  if(*is_thread_pred == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start watchers", ImVec2(100,0))){
      ioManager->start_dirWatcher();
    }
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(133, 45, 45, 255));
    if(ImGui::Button("Stop watchers", ImVec2(100,0))){
      ioManager->stop_dirWatcher();
    }
  }
  ImGui::PopStyleColor(1);

  //Prediction directory
  if(ImGui::Button("...##1")){
    ioManager->select_dir_path();
  }
  ImGui::SameLine();
  string dir_path = ioManager->get_dir_path();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

  //---------------------------
}
void GUI_Obstacle::make_obstacleName(Subset* subset){
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
void GUI_Obstacle::parameters(){
  //---------------------------
/*
  //Dicrectory path selection & display
  if(ImGui::Button("...##26")){
    ioManager->save_image_path();
  }
  ImGui::SameLine();
  string saveas = *ioManager->get_save_image_path();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());
*/
  //bool* with_save_image = ioManager->get_with_save_image();
  //ImGui::Checkbox("Save image", with_save_image);

  //---------------------------
}
