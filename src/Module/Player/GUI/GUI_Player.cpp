#include "GUI_Player.h"

#include "GUI_Dynamic.h"
#include "GUI_Obstacle.h"

#include "../../Module_GUI.h"


//Constructor / Destructor
GUI_Player::GUI_Player(GUI_module* gui_module){
  //---------------------------

  this->gui_dynamic = new GUI_Dynamic(gui_module);
  this->gui_obstacle = new GUI_Obstacle(gui_module);

  this->item_width = 100;
  
  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::design_player(){
  //---------------------------

  if(ImGui::BeginTabBar("Player##156", ImGuiTabBarFlags_None)){
    //-------------------------------

    // Obstacle detection
    if(ImGui::BeginTabItem("Obstacle")){
      gui_obstacle->design_Obstacle();
      ImGui::EndTabItem();
    }

    // Offline cloud player
    if(ImGui::BeginTabItem("Dynamic")){
      gui_dynamic->design_dynamic();
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  //---------------------------
}
void GUI_Player::runtime(){
  //---------------------------

  gui_obstacle->runtime_display_naming();
  gui_dynamic->runtime_player_mouse();

  //---------------------------
}
