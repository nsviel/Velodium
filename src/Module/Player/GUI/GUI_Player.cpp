#include "GUI_Player.h"

#include "../Dynamic/GUI/GUI_Dynamic.h"
#include "../Obstacle/GUI/GUI_Obstacle.h"

#include "../../Module_GUI.h"


//Constructor / Destructor
GUI_Player::GUI_Player(GUI_module* node_gui_module){
  //---------------------------

  this->gui_dynamic = new GUI_Dynamic(node_gui_module);
  this->gui_obstacle = new GUI_Obstacle(node_gui_module);

  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::design_player(){
  //---------------------------

  if(ImGui::BeginTabBar("Player##156", ImGuiTabBarFlags_None)){
    //-------------------------------

    // Offline cloud player
    if(ImGui::BeginTabItem("Dynamic")){
      gui_dynamic->design_dynamic();
      ImGui::EndTabItem();
    }

    // Obstacle detection
    if(ImGui::BeginTabItem("Obstacle")){
      gui_obstacle->design_Obstacle();
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  //---------------------------
}
void GUI_Player::runtime(){
  //---------------------------

  gui_obstacle->compute_display_naming();
  gui_dynamic->runtime_player_mouse();

  //---------------------------
}
