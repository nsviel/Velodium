#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "LiDAR/GUI/GUI_Lidar.h"
#include "Player/GUI/GUI_Player.h"
#include "SLAM/GUI/GUI_Slam.h"
#include "Network/GUI/GUI_Network.h"
#include "Obstacle/GUI/GUI_Obstacle.h"

#include "../Engine/Configuration/config_module.h"
#include "../common.h"


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(Engine* engineManager){
    //-------------------------------

    this->gui_lidarManager = new GUI_Lidar();
    this->gui_playerManager = new GUI_Player(engineManager);
    this->gui_slamManager = new GUI_Slam(engineManager);
    this->gui_senderManager = new GUI_Network();
    this->gui_obstacleManager = new GUI_Obstacle(engineManager);

    this->module_velodyne = true;
    this->module_player = true;
    this->module_slam = true;
    this->module_obstacle = true;
    this->module_scala = false;

    //-------------------------------
    this->init_configuration();
  }
  ~GUI_module();

public:
  void init_configuration(){
    //-------------------------------

    this->configManager = new config_module();
    configManager->make_configuration();

    //-------------------------------
  }
  void display_moduleTabs(){
    if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
      //-------------------------------

      // Network stuff management
      if(module_velodyne){
        if(ImGui::BeginTabItem("Network")){
          gui_senderManager->design_Network();
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
      gui_playerManager->player_mouse();

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

      // Obstacle detection
      if(module_obstacle){
        if(ImGui::BeginTabItem("Obstacle")){
          gui_obstacleManager->design_Obstacle();
          ImGui::EndTabItem();
        }
      }

      //-------------------------------
      ImGui::EndTabBar();
    }
  }

  inline GUI_Slam* get_gui_slamManager(){return gui_slamManager;}
  inline GUI_Player* get_gui_player(){return gui_playerManager;}

private:
  config_module* configManager;

  GUI_Lidar* gui_lidarManager;
  GUI_Player* gui_playerManager;
  GUI_Slam* gui_slamManager;
  GUI_Network* gui_senderManager;
  GUI_Obstacle* gui_obstacleManager;

  bool module_velodyne;
  bool module_player;
  bool module_slam;
  bool module_obstacle;
  bool module_scala;
};

#endif
