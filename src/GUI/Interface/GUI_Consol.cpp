#include "GUI_Consol.h"
#include "struct_consol.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Configuration/Dimension.h"
#include "../../Engine/Configuration/Configuration.h"

#include "../../../extern/imgui/imgui.h"

#include <thread>

extern struct ConsoleApp console;


//Constructor / Destructor
GUI_consol::GUI_consol(Engine* renderer){
  this->engineManager = renderer;
  //----------------------------

  this->dimManager = engineManager->get_dimManager();

  Configuration configManager;
  this->panel_X = 300;
  this->panel_Y = configManager.parse_json_int("gui", "botPanel_height");

  //----------------------------
}
GUI_consol::~GUI_consol(){}

//Main function
void GUI_consol::design_consol(){
  float gui_leftPanel_w = dimManager->get_gui_leftPanel_width();
  vec2 winDim = dimManager->get_win_dim();
  //----------------------------

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(gui_leftPanel_w, winDim.y - panel_Y));
  ImGui::SetNextWindowSize(ImVec2(winDim.x - gui_leftPanel_w, panel_Y));
  ImGui::Begin("BottomPanel##outer", NULL, window_flags);{

    //Update panel dimension
    panel_X = ImGui::GetWindowSize().x;
    panel_Y = ImGui::GetWindowSize().y;

    //Set inner window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(gui_leftPanel_w, winDim.y - panel_Y + 1));
    ImGui::SetNextWindowSize(ImVec2(winDim.x - gui_leftPanel_w, panel_Y - 1));
    ImGui::Begin("BottomPanel##inner", NULL, window_flags);{

      //Draw console
      console.Draw();

    }
    ImGui::PopStyleVar();
  }

  ImGui::End();

  //----------------------------
}
