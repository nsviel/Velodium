#include "Module_GUI.h"

#include "../GUI/GUI_node.h"
#include "../Engine/Engine_node.h"
#include "../Operation/Node_operation.h"


//Constructor / Destructor
GUI_module::GUI_module(GUI_node* node_gui){
  //-------------------------------

  this->node_module = node_gui->get_node_module();
  this->node_engine = node_gui->get_node_engine();
  this->node_ope = node_gui->get_node_ope();

  //---------------------------
}
GUI_module::~GUI_module(){}

void GUI_module::display_moduleTabs(){
  if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
    //-------------------------------

    ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(0, 0, 0, 255));

    // SLAM
    /*if(module_slam){
      if(ImGui::BeginTabItem("SLAM")){
        ImGui::EndTabItem();
      }
    }*/

    ImGui::PopStyleColor();

    //-------------------------------
    ImGui::EndTabBar();
  }
}
