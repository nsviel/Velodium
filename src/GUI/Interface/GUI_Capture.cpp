#include "GUI_Capture.h"
#include "GUI_Network.h"
#include "GUI_Scala.h"
#include "GUI_Velodyne.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/IO/Recorder.h"
#include "../../Interface/File/Path.h"
#include "../../Interface/Capture/Capture.h"
#include "../../Interface/Capture/LiDAR/Scala/Scala.h"
#include "../../Interface/Capture/LiDAR/Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Capture::GUI_Capture(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->captureManager = node_interface->get_captureManager();
  this->recordManager = node_interface->get_recordManager();
  this->gui_network = node_gui->get_gui_network();
  this->gui_scala = new GUI_Scala(node_gui);
  this->gui_velodyne = new GUI_Velodyne(node_gui);

  this->item_width = 100;

  //---------------------------
}
GUI_Capture::~GUI_Capture(){}

//Main function
void GUI_Capture::design_interface(){
  if(ImGui::BeginTabItem("Interface")){
    if(ImGui::BeginTabBar("##Interface", ImGuiTabBarFlags_None)){
      //---------------------------

      this->design_capture();
      gui_network->design_Network();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Capture::design_capture(){
  if(ImGui::BeginTabItem("Capture")){
    if(ImGui::BeginTabBar("##tabs_capture", ImGuiTabBarFlags_None)){
      //---------------------------

      if(ImGui::BeginTabItem("Recorder")){
        this->design_recorder();
        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("Velodyne")){
        gui_velodyne->design_Velodyne();
        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("Scala")){
        gui_scala->design_Scala();
        ImGui::EndTabItem();
      }

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Capture::design_recorder(){
  //---------------------------

  this->state_watcher();
  this->recorder_frame();
  this->recorder_screenshot();

  //---------------------------
}

//Specific functions
void GUI_Capture::state_watcher(){
  //---------------------------

  gui_velodyne->velo_state();
  gui_scala->scala_state();

  //---------------------------
  ImGui::Separator();
}
void GUI_Capture::recorder_frame(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Frame");
  //---------------------------

  //Save frame in folder for AI module
  bool* with_save_frame = recordManager->get_with_save_frame();
  ImGui::Checkbox("Activated##1", with_save_frame);

  // set save image number
  static bool with_number = true;
  if(ImGui::Checkbox("##12323", &with_number)){
    recordManager->select_frame_unlimited(!with_number);
  }
  ImGui::SameLine();

  // If not set number of image
  int* save_frame_max = recordManager->get_save_frame_max();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Number##1", save_frame_max);

  //Path where images are saved
  if(ImGui::Button("...##23")){
    recordManager->select_path_frame();
  }
  ImGui::SameLine();
  string path = get_path_abs(recordManager->get_path_frame());
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_Capture::recorder_screenshot(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Screenshot");
  //---------------------------

  //Save image for interfacing
  bool* with_save_image = recordManager->get_with_save_image();
  ImGui::Checkbox("Activated##2", with_save_image);

  // set save image number
  static bool with_number = true;
  if(ImGui::Checkbox("##1232", &with_number)){
    recordManager->select_image_unlimited(!with_number);
  }
  ImGui::SameLine();

  // If not set number of image
  int* save_image_max = recordManager->get_save_image_max();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Number##2", save_image_max);

  //Path where images are saved
  if(ImGui::Button("...##25")){
    recordManager->select_path_image();
  }
  ImGui::SameLine();
  string path = get_path_abs(recordManager->get_path_image());
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());

  //---------------------------
  ImGui::Separator();
}
