#include "GUI_LeftPanel.h"

#include "../Windows/GUI_windows.h"
#include "../Node/GUI_FileManager.h"

#include "../../Module/GUI_Module.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Configuration/Dimension.h"
#include "../../Engine/Configuration/Configuration.h"

#include "../../../extern/imgui/imgui.h"


//Constructor / Destructor
GUI_leftPanel::GUI_leftPanel(Engine* renderer, GUI_windows* window, GUI_module* module){
  this->gui_winManager = window;
  this->engineManager = renderer;
  this->gui_moduleManager = module;
  //-------------------------------

  this->dimManager = engineManager->get_dimManager();
  this->gui_fileManager = new GUI_fileManager(dimManager, gui_winManager);

  Configuration configManager;
  this->panel_X = configManager.parse_json_int("gui", "leftPanel_width");
  this->panel_Y = configManager.parse_json_int("gui", "leftPanel_mid");

  //-------------------------------
}
GUI_leftPanel::~GUI_leftPanel(){}

//Main function
void GUI_leftPanel::design_leftPanel(){
  //----------------------------

  this->panel_top();
  this->panel_bot();

  //----------------------------
}

void GUI_leftPanel::panel_top(){
  float topPanel_h = dimManager->get_gui_topPanel_height();
  vec2 winDim = dimManager->get_win_dim();
  ImVec2 lp_min = ImVec2(10, 10);
  ImVec2 lp_max = ImVec2(500, winDim.y - topPanel_h);
  //----------------------------

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(0, topPanel_h));
  ImGui::SetNextWindowSize(ImVec2(panel_X, panel_Y - topPanel_h));
  ImGui::SetNextWindowSizeConstraints(lp_min, lp_max);
  ImGui::Begin("LeftPanel##topOuter", NULL, window_flags);{

    //Update panel dimension
    panel_X = ImGui::GetWindowSize().x;
    panel_Y = ImGui::GetWindowSize().y + topPanel_h;

    //Set inner window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(0, topPanel_h));
    ImGui::SetNextWindowSize(ImVec2(panel_X - 1, panel_Y - topPanel_h - 1));
    ImGui::Begin("LeftPanel##topInner", NULL, window_flags);{

    //File management
    gui_fileManager->fileManager();

    }
    ImGui::PopStyleVar();
  }

  ImGui::End();

  //----------------------------
}
void GUI_leftPanel::panel_bot(){
  //----------------------------

  //Parameters
  float topPanel_h = dimManager->get_gui_topPanel_height();
  vec2 winDim = dimManager->get_win_dim();
  ImVec2 lp_min = ImVec2(10, 10);
  ImVec2 lp_max = ImVec2(500, winDim.y - topPanel_h);

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(0, panel_Y));
  ImGui::SetNextWindowSize(ImVec2(panel_X, winDim.y - panel_Y));
  ImGui::SetNextWindowSizeConstraints(lp_min, lp_max);
  ImGui::Begin("LeftPanel##botOuter", NULL, window_flags);
  //---------------

  //Update panel dimension
  panel_X = ImGui::GetWindowSize().x;
  panel_Y = winDim.y - ImGui::GetWindowSize().y;

  //Set inner window
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
  ImGui::SetNextWindowPos(ImVec2(0, panel_Y + 1));
  ImGui::SetNextWindowSize(ImVec2(panel_X - 1, winDim.y - panel_Y - 1));
  ImGui::Begin("LeftPanel##botInner", NULL, window_flags);

  //Working areas
  gui_moduleManager->display_moduleTabs();

  ImGui::PopStyleVar();

  //---------------
  ImGui::End();

  //----------------------------
}
