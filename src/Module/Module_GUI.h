#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "LiDAR/GUI/GUI_Lidar.h"
#include "Player/GUI/GUI_Player.h"
#include "SLAM/GUI/GUI_Slam.h"
#include "Network/GUI/GUI_Network.h"
#include "Obstacle/GUI/GUI_Obstacle.h"

#include "../GUI/GUI_node.h"
#include "../Engine/Configuration/config_module.h"
#include "../common.h"


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(GUI_node* node_gui){
    //-------------------------------

    this->gui_lidarManager = new GUI_Lidar(node_gui);
    this->gui_playerManager = new GUI_Player(node_gui);
    this->gui_slamManager = new GUI_Slam(node_gui);
    this->gui_netManager = new GUI_Network(node_gui);
    this->gui_obstacleManager = new GUI_Obstacle(node_gui);

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

      // Obstacle detection
      if(module_obstacle){
        if(ImGui::BeginTabItem("Obstacle")){
          gui_obstacleManager->design_Obstacle();
          ImGui::EndTabItem();
        }
      }

      // Network stuff management
      if(module_velodyne){
        if(ImGui::BeginTabItem("Network")){
          gui_netManager->design_Network();
          ImGui::EndTabItem();
        }
      }

      // Velodyne data management
      if(module_player){
        if(ImGui::BeginTabItem("Player")){
          gui_playerManager->design_player_cloud();
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

      // LiDAR management
      if(module_velodyne){
        if(ImGui::BeginTabItem("LiDAR")){
          gui_lidarManager->design_Velodyne();
          gui_lidarManager->design_Scala();
          ImGui::EndTabItem();
        }
      }

      //-------------------------------
      ImGui::EndTabBar();
    }
  }
  void runtime(){
    //-------------------------------

    gui_playerManager->player_mouse();

    //-------------------------------
  }

  inline GUI_Slam* get_gui_slamManager(){return gui_slamManager;}
  inline GUI_Lidar* get_gui_lidarManager(){return gui_lidarManager;}
  inline GUI_Player* get_gui_playerManager(){return gui_playerManager;}
  inline GUI_Network* get_gui_netManager(){return gui_netManager;}

private:
  GUI_Lidar* gui_lidarManager;
  GUI_Player* gui_playerManager;
  GUI_Slam* gui_slamManager;
  GUI_Network* gui_netManager;
  GUI_Obstacle* gui_obstacleManager;

  bool module_velodyne;
  bool module_player;
  bool module_slam;
  bool module_obstacle;
  bool module_scala;
};

#endif
