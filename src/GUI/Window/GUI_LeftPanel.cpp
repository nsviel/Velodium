#include "GUI_LeftPanel.h"

#include "../Node_gui.h"
#include "../Box/GUI_windows.h"
#include "../Control/GUI_FileManager.h"
#include "../Interface/GUI_Capture.h"
#include "../Dynamic/GUI_Online.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Dimension.h"

#include "../../Module/Node_module.h"

#include "imgui/imgui.h"


//Constructor / Destructor
GUI_leftPanel::GUI_leftPanel(Node_gui* node){
  this->node_gui = node;
  //-------------------------------

  Node_engine* node_engine = node_gui->get_node_engine();

  this->node_module = node_engine->get_node_module();
  this->dimManager = node_engine->get_dimManager();
  this->gui_window = node_gui->get_gui_window();
  this->gui_fileManager = node_gui->get_gui_fileManager();
  this->gui_online = node_gui->get_gui_online();
  this->gui_capture = node_gui->get_gui_capture();

  //-------------------------------
}
GUI_leftPanel::~GUI_leftPanel(){}

//Main function
void GUI_leftPanel::design_leftPanel(){
  //----------------------------

  this->panel_top();
  this->panel_bot();
  this->update_dimension();

  //----------------------------
}
void GUI_leftPanel::design_leftPanel_bottom(){
  if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
    ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(0, 0, 0, 255));
    //-------------------------------

    gui_online->design_dynamic();
    gui_capture->design_interface();
    node_module->draw();

    //-------------------------------
    ImGui::PopStyleColor();
    ImGui::EndTabBar();
  }
}

//Sub-function
void GUI_leftPanel::panel_top(){
  //----------------------------

  //Get panel dimension
  vec2 win_dim = dimManager->get_win_dim();
  vec2* ltp_dim = dimManager->get_gui_ltp_dim();
  vec2* ltp_pos = dimManager->get_gui_ltp_pos();

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(ltp_pos->x, ltp_pos->y));
  ImGui::SetNextWindowSize(ImVec2(ltp_dim->x, ltp_dim->y));
  ImGui::SetNextWindowSizeConstraints(ImVec2(2, 10), ImVec2(500, win_dim.y));
  ImGui::Begin("LeftPanel##topOuter", NULL, window_flags);{

    //Update panel dimension
    dim_ltp.x = ImGui::GetWindowSize().x;
    dim_ltp.y = ImGui::GetWindowSize().y;

    //Set inner window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(ltp_pos->x+1, ltp_pos->y+1));
    ImGui::SetNextWindowSize(ImVec2(ltp_dim->x-2, ltp_dim->y-2));
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
  vec2 win_dim = dimManager->get_win_dim();
  //----------------------------

  //Get panel dimension
  vec2* lbp_dim = dimManager->get_gui_lbp_dim();
  vec2* lbp_pos = dimManager->get_gui_lbp_pos();

  //Options
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImVec2(lbp_pos->x, lbp_pos->y));
  ImGui::SetNextWindowSize(ImVec2(lbp_dim->x, lbp_dim->y));
  ImGui::SetNextWindowSizeConstraints(ImVec2(1, 10), ImVec2(500, win_dim.y));
  ImGui::Begin("LeftPanel##botOuter", NULL, window_flags);
  //---------------

  //Update panel dimension
  dim_lbp.x = ImGui::GetWindowSize().x;
  dim_lbp.y = ImGui::GetWindowSize().y;

  //Set inner window
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
  ImGui::SetNextWindowPos(ImVec2(lbp_pos->x+1, lbp_pos->y+1));
  ImGui::SetNextWindowSize(ImVec2(lbp_dim->x-2, lbp_dim->y-2));
  ImGui::Begin("LeftPanel##botInner", NULL, window_flags);

  //Working areas
  this->design_leftPanel_bottom();

  ImGui::PopStyleVar();

  //---------------
  ImGui::End();

  //----------------------------
}
void GUI_leftPanel::update_dimension(){
  //----------------------------

  vec2 win_dim = dimManager->get_win_dim();
  vec2* tp_dim = dimManager->get_gui_tp_dim();
  vec2* ltp_dim = dimManager->get_gui_ltp_dim();
  vec2* ltp_pos = dimManager->get_gui_ltp_pos();
  vec2* lbp_dim = dimManager->get_gui_lbp_dim();
  vec2* lbp_pos = dimManager->get_gui_lbp_pos();
  vec2* bp_pos = dimManager->get_gui_bp_pos();
  vec2* bp_dim = dimManager->get_gui_bp_dim();

  //Top panel change
  if(dim_ltp.x != ltp_dim->x || dim_ltp.y != ltp_dim->y){
    *ltp_dim = dim_ltp;

    lbp_dim->x = dim_ltp.x;
    lbp_pos->y = dim_ltp.y + tp_dim->y;
    lbp_dim->y = win_dim.y - dim_ltp.y - tp_dim->y;
    bp_pos->x = dim_ltp.x;
    bp_dim->x = win_dim.x - dim_ltp.x;

    dimManager->update();
  }
  //Bottom panel change
  else if(dim_lbp.x != lbp_dim->x || dim_lbp.y != lbp_dim->y){
    *lbp_dim = dim_lbp;

    ltp_dim->x = dim_lbp.x;
    ltp_dim->y = win_dim.y - dim_lbp.y - tp_dim->y;
    lbp_pos->y = dim_ltp.y + tp_dim->y;
    bp_pos->x = dim_lbp.x;
    bp_dim->x = win_dim.x - dim_lbp.x;

    dimManager->update();
  }

  //----------------------------
}
