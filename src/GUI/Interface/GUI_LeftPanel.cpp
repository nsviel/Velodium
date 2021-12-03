#include "GUI_LeftPanel.h"
#include "GUI_windows.h"

#include "../Node/GUI_FileManager.h"

#include "../../Module/GUI_Module.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Data/Database.h"
#include "../../Engine/Configuration/Dimension.h"

#include "../../../extern/imgui/imgui.h"

extern struct Database database;


//Constructor / Destructor
GUI_leftPanel::GUI_leftPanel(Engine* renderer, GUI_windows* winManager){
  this->gui_winManager = winManager;
  this->engineManager = renderer;
  //-------------------------------

  this->dimManager = engineManager->get_dimManager();

  this->gui_moduleManager = new GUI_module(engineManager->get_CameraManager());
  this->gui_fileManager = new GUI_fileManager(dimManager, winManager);

  this->panel_X = configuration.GUI_LeftPanel_width;
  this->panel_Y = configuration.GUI_LeftPanel_mid;

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
  vec2 guiDim_tP = dimManager->get_guiDim_tP();
  vec2 winDim = dimManager->get_winDim();
  ImVec2 lp_min = ImVec2(10, 10);
  ImVec2 lp_max = ImVec2(500, winDim.y - guiDim_tP.y);
  //----------------------------

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(0, guiDim_tP.y));
  ImGui::SetNextWindowSize(ImVec2(panel_X, panel_Y - guiDim_tP.y));
  ImGui::SetNextWindowSizeConstraints(lp_min, lp_max);
  ImGui::Begin("LeftPanel##topOuter", NULL, window_flags);{

    //Update panel dimension
    panel_X = ImGui::GetWindowSize().x;
    panel_Y = ImGui::GetWindowSize().y + guiDim_tP.y;

    //Set inner window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(0, guiDim_tP.y));
    ImGui::SetNextWindowSize(ImVec2(panel_X - 1, panel_Y - guiDim_tP.y - 1));
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
  vec2 guiDim_tP = dimManager->get_guiDim_tP();
  vec2 winDim = dimManager->get_winDim();
  ImVec2 lp_min = ImVec2(10, 10);
  ImVec2 lp_max = ImVec2(500, winDim.y - guiDim_tP.y);

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
  ImGui::Begin("LeftPanel##botInner", NULL, window_flags);{

  //Working areas
  gui_moduleManager->display_moduleTabs();

  }
  ImGui::PopStyleVar();

  //---------------
  ImGui::End();

  //----------------------------
}
