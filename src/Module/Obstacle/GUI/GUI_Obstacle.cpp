#include "GUI_Obstacle.h"

#include "../Obstacle.h"

#include "../../../Engine/Scene.h"


//Constructor / Destructor
GUI_Obstacle::GUI_Obstacle(Engine* engineManager){
  //---------------------------

  this->coordManager = new CoordTransform(engineManager->get_CameraManager(), engineManager->get_dimManager());
  this->obstacleManager = new Obstacle();
  this->sceneManager = new Scene();

  //---------------------------
}
GUI_Obstacle::~GUI_Obstacle(){}

void GUI_Obstacle::design_Obstacle(){
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  this->run();
  this->make_obstacleName(subset);

  //---------------------------
}

void GUI_Obstacle::run(){
  //---------------------------

  if(ImGui::Button("run", ImVec2(100,0))){
    obstacleManager->run();
  }

  //Prediction directory
  if(ImGui::Button("...##1")){
    obstacleManager->select_dir_path();
  }
  ImGui::SameLine();
  string dir_path = obstacleManager->get_dir_path();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", dir_path.c_str());

  //---------------------------
}
void GUI_Obstacle::make_obstacleName(Subset* subset){
  if(subset == nullptr) return;
  label_ID = 0;
  //---------------------------

  Obstac* obstacle = &subset->obstacle;

  for(int j=0; j<obstacle->pr_name.size(); j++){
    string name = obstacle->pr_name[j];
    vec3 position = obstacle->pr_position[j];
    position.z = obstacle->pr_dimension[j].z;

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
  //window_flags |= ImGuiWindowFlags_NoNav;
  //window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;

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
    //ImGui::SetWindowSize(ImVec2(100, 10));

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
