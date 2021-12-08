#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "Velodyne/GUI_Velodyne.h"
#include "Scala/GUI_Scala.h"
#include "CloudPlayer/GUI_CloudPlayer.h"
#include "SLAM/GUI/GUI_Slam.h"
#include "Network/GUI_Network.h"

#include "../common.h"


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(Camera* cameraManager){
    this->gui_veloManager = new GUI_Velodyne();
    this->gui_scalaManager = new GUI_Scala();
    this->gui_odomManager = new GUI_CloudPlayer(cameraManager);
    this->gui_slamManager = new GUI_Slam();
    this->gui_senderManager = new GUI_Network();

    this->module_velodyne = true;
    this->module_CloudPlayer = true;
    this->module_slam = true;
  }
  ~GUI_module();

public:
  void display_moduleTabs(){
    if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
      //-------------------------------

      // SLAM
      if(module_slam){
        if(ImGui::BeginTabItem("SLAM")){
          gui_slamManager->design_SLAM();
          ImGui::EndTabItem();
        }
      }

      // Velodyne data management
      if(module_CloudPlayer){
        if(ImGui::BeginTabItem("Player")){
          gui_odomManager->design_CloudPlayer();
          ImGui::EndTabItem();
        }
      }
      gui_odomManager->playCloud_byMouseWheel();

      // Velodyne data management
      if(module_velodyne){
        if(ImGui::BeginTabItem("Velodyne")){
          gui_veloManager->design_Velodyne();
          ImGui::EndTabItem();
        }
      }

      // Scala LiDAR management
      if(module_velodyne){
        if(ImGui::BeginTabItem("Scala")){
          gui_scalaManager->design_Scala();
          ImGui::EndTabItem();
        }
      }

      // Network stuff management
      if(module_velodyne){
        if(ImGui::BeginTabItem("Network")){
          gui_senderManager->design_Network();
          ImGui::EndTabItem();
        }
      }

      //-------------------------------
      ImGui::EndTabBar();
    }
  }

private:
  GUI_Velodyne* gui_veloManager;
  GUI_Scala* gui_scalaManager;
  GUI_CloudPlayer* gui_odomManager;
  GUI_Slam* gui_slamManager;
  GUI_Network* gui_senderManager;

  bool module_velodyne;
  bool module_CloudPlayer;
  bool module_slam;

};

#endif
