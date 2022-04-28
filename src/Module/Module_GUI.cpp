#include "Module_GUI.h"

#include "SLAM/GUI/GUI_Slam.h"
#include "Interface/GUI/GUI_Interface.h"
#include "Player/GUI/GUI_Player.h"

#include "../GUI/GUI_node.h"
#include "../Engine/Engine_node.h"
#include "../Operation/Operation_node.h"
#include "../Operation/Operation_GUI.h"


//Constructor / Destructor
GUI_module::GUI_module(GUI_node* node){
  this->node_gui = node;
  //-------------------------------

  this->node_module = node_gui->get_node_module();
  this->node_engine = node_gui->get_node_engine();
  this->node_ope = node_gui->get_node_ope();

  this->gui_operation = node_gui->get_gui_operation();
  this->gui_interface = new GUI_Interface(this);
  this->gui_player = new GUI_Player(this);
  this->gui_slam = new GUI_Slam(this);

  this->module_velodyne = true;
  this->module_player = true;
  this->module_slam = true;
  this->module_obstacle = true;
  this->module_scala = false;

  //---------------------------
}
GUI_module::~GUI_module(){}

void GUI_module::display_moduleTabs(){
  if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
    //-------------------------------

    ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(0, 0, 0, 255));

    // Dynamic & obstacle stuff
    if(module_player){
      if(ImGui::BeginTabItem("Player")){
        gui_player->design_player();
        ImGui::EndTabItem();
      }
    }

    // Interface stuff
    if(module_velodyne){
      if(ImGui::BeginTabItem("Interface")){
        gui_interface->design_Interface();
        ImGui::EndTabItem();
      }
    }

    // SLAM
    if(module_slam){
      if(ImGui::BeginTabItem("SLAM")){
        gui_slam->design_SLAM();
        ImGui::EndTabItem();
      }
    }

    ImGui::PopStyleColor();

    //-------------------------------
    ImGui::EndTabBar();
  }
}
void GUI_module::runtime(){
  //-------------------------------

  gui_player->runtime();

  //-------------------------------
}
