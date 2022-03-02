#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "SLAM/GUI/GUI_Slam.h"
#include "Interface/GUI/GUI_Interface.h"
#include "Player/GUI/GUI_Player.h"

#include "../GUI/GUI_node.h"
#include "../Engine/Engine_node.h"
#include "../Operation/Operation_node.h"
#include "../common.h"


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(GUI_node* node){
    this->node_gui = node;
    //-------------------------------

    this->node_module = node_gui->get_node_module();
    this->node_engine = node_gui->get_node_engine();
    this->node_ope = node_gui->get_node_ope();

    this->gui_playerManager = new GUI_Player(this);
    this->gui_slamManager = new GUI_Slam(this);
    this->gui_node_interface = new GUI_Interface(this);

    this->module_velodyne = true;
    this->module_player = true;
    this->module_slam = true;
    this->module_obstacle = true;
    this->module_scala = false;

    //-------------------------------
  }
  ~GUI_module();

public:
  void display_moduleTabs(){
    if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
      //-------------------------------

      ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(0, 0, 0, 255));

      // Dynamic & obstacle stuff
      if(module_player){
        if(ImGui::BeginTabItem("Player")){
          gui_playerManager->design_player();
          ImGui::EndTabItem();
        }
      }

      // Interface stuff
      if(module_velodyne){
        if(ImGui::BeginTabItem("Interface")){
          gui_node_interface->design_Interface();
          ImGui::EndTabItem();
        }
      }

      // SLAM
      if(module_slam){
        if(ImGui::BeginTabItem("SLAM")){
          gui_slamManager->design_SLAM();
          ImGui::EndTabItem();
        }
      }

      ImGui::PopStyleColor();

      //-------------------------------
      ImGui::EndTabBar();
    }
  }
  void runtime(){
    //-------------------------------

    gui_playerManager->runtime();

    //-------------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}

  inline GUI_node* get_node_gui(){return node_gui;}
  inline GUI_Slam* get_gui_slamManager(){return gui_slamManager;}
  inline GUI_Player* get_gui_playerManager(){return gui_playerManager;}
  inline GUI_Interface* get_gui_node_interface(){return gui_node_interface;}

private:
  Module_node* node_module;
  Engine_node* node_engine;
  Operation_node* node_ope;

  GUI_node* node_gui;
  GUI_Player* gui_playerManager;
  GUI_Slam* gui_slamManager;
  GUI_Interface* gui_node_interface;

  bool module_velodyne;
  bool module_player;
  bool module_slam;
  bool module_obstacle;
  bool module_scala;
};

#endif
