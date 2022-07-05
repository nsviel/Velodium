#include "Player_GUI.h"

#include "GUI/GUI_Dynamic.h"
#include "GUI/GUI_Obstacle.h"
#include "GUI/GUI_State.h"

#include "../Module_GUI.h"


//Constructor / Destructor
GUI_Player::GUI_Player(GUI_module* gui_module){
  //---------------------------

  this->gui_dynamic = new GUI_Dynamic(gui_module);
  this->gui_obstacle = new GUI_Obstacle(gui_module);
  this->gui_state = new GUI_State(gui_module);

  this->item_width = 100;

  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::design_player(){
  //---------------------------

  if(ImGui::BeginTabBar("Obstacle##156", ImGuiTabBarFlags_None)){
    //-------------------------------

    // Obstacle detection
    if(ImGui::BeginTabItem("State")){
      gui_state->design_state();
      ImGui::EndTabItem();
    }

    // Obstacle detection
    if(ImGui::BeginTabItem("Online")){
      gui_obstacle->design_obstacle();
      ImGui::EndTabItem();
    }

    // Offline cloud player
    if(ImGui::BeginTabItem("Player")){
      gui_dynamic->design_dynamic();
      ImGui::EndTabItem();
    }

    // Operation time
    if(ImGui::BeginTabItem("Time")){
      gui_state->design_time();
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
