#include "GUI_Consol.h"
#include "struct_consol.h"

#include "../../GUI/Node_gui.h"
#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Dimension.h"
#include "../../Engine/Scene/Scene.h"

#include "imgui/imgui.h"

#include <thread>

//Consol creation
ConsoleApp console;


//Constructor / Destructor
GUI_consol::GUI_consol(Node_gui* node_gui){
  //----------------------------

  Node_engine* node_engine = node_gui->get_node_engine();

  this->dimManager = node_engine->get_dimManager();

  //----------------------------
}
GUI_consol::~GUI_consol(){}

//Main function
void GUI_consol::design_consol(){
  //----------------------------

  this->draw_consol();
  this->update_dimension();

  //----------------------------
}
void GUI_consol::draw_consol(){
  //----------------------------

  //Get panel dimension
  vec2 win_dim = dimManager->get_win_dim();
  vec2* bp_pos = dimManager->get_gui_bp_pos();
  vec2* bp_dim = dimManager->get_gui_bp_dim();

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(bp_pos->x, bp_pos->y));
  ImGui::SetNextWindowSize(ImVec2(bp_dim->x, bp_dim->y));
  ImGui::Begin("BottomPanel##outer", NULL, window_flags);{

    //Update panel dimension
    dim_bp.x = ImGui::GetWindowSize().x;
    dim_bp.y = ImGui::GetWindowSize().y;

    //Set inner window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(bp_pos->x+1, bp_pos->y+1));
    ImGui::SetNextWindowSize(ImVec2(bp_dim->x-2, bp_dim->y-2));
    ImGui::Begin("BottomPanel##inner", NULL, window_flags);{

      //Draw console
      console.Draw();

    }
    ImGui::PopStyleVar();
  }

  ImGui::End();

  //----------------------------
}
void GUI_consol::update_dimension(){
  //----------------------------

  //Get panel dimension
  vec2 win_dim = dimManager->get_win_dim();
  vec2* bp_pos = dimManager->get_gui_bp_pos();
  vec2* bp_dim = dimManager->get_gui_bp_dim();

  if(dim_bp.y != bp_dim->y){
    bp_dim->y = dim_bp.y;
    bp_pos->y = win_dim.y - dim_bp.y;

    dimManager->update();
  }

  //----------------------------
}
