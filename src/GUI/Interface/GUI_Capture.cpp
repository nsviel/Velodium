#include "GUI_Capture.h"
#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"
#include "../../Interface/Capture/LiDAR/Scala/Scala.h"
#include "../../Interface/Capture/LiDAR/Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Capture::GUI_Capture(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->captureManager = node_interface->get_captureManager();
  this->scalaManager = captureManager->get_scalaManager();
  this->veloManager = captureManager->get_veloManager();
  this->gui_network = node_gui->get_gui_network();

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

      if(ImGui::BeginTabItem("Velodyne")){
        this->design_Velodyne();
        ImGui::EndTabItem();
      }

      if(ImGui::BeginTabItem("Scala")){
        this->design_Scala();
        ImGui::EndTabItem();
      }

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}

//Specific functions
void GUI_Capture::design_Velodyne(){
  //---------------------------

  this->velo_state();
  this->velo_capture();
  this->velo_parameter();

  //---------------------------
  ImGui::Separator();
}
void GUI_Capture::design_Scala(){
  //---------------------------

  this->scala_state();
  this->scala_capture();
  this->scala_file();
  this->scala_parameter();

  //---------------------------
  ImGui::Separator();
}
void GUI_Capture::state_watcher(){
  //---------------------------

  //Watchers Velodyne
  bool is_velodyne_watcher = *captureManager->get_is_capturing();
  ImGui::Text("Watcher - Velodyne");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_velodyne_watcher ? "ON" : "OFF");

  //Watcher Scala
  bool is_scala_watcher = *captureManager->get_is_capturing();
  ImGui::Text("Watcher - Scala");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_scala_watcher ? "ON" : "OFF");

  //---------------------------
  ImGui::Separator();
}

//Velodyne subfunctions
void GUI_Capture::velo_state(){
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Velodyne");

  //Capture time
  ImGui::Text("Capture time");
  ImGui::SameLine();
  int capture_time = captureManager->get_capture_time();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d ms", capture_time);

  //Number of points
  ImGui::Text("Number of points");
  ImGui::SameLine();
  int capture_nb_point_raw = captureManager->get_capture_nb_point_raw();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d", capture_nb_point_raw);
  ImGui::Text("Number of non-zero points");
  ImGui::SameLine();
  int capture_nb_point = captureManager->get_capture_nb_point();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d", capture_nb_point);

  //Motor state
  ImGui::Text("State");
  ImGui::SameLine();
  int rot_freq = veloManager->get_rot_freq();
  int rot_rpm = veloManager->get_rot_rpm();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d Hz", rot_freq);
  ImGui::SameLine();
  ImGui::Text(" | ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d rpm", rot_rpm);

  //---------------------------
}
void GUI_Capture::velo_capture(){
  bool is_capturing = *veloManager->get_is_capturing();
  //---------------------------

  //Capturing button
  if(is_capturing == false){
    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start capture", ImVec2(157, 0))){
      captureManager->start_new_capture("velodyne_vlp16");
    }
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop capture", ImVec2(157, 0))){
      captureManager->stop_capture();
    }
  }
  ImGui::PopStyleColor(1);

  //LiDAR motor
  if(ImGui::Button("Start motor", ImVec2(75, 0))){
    veloManager->lidar_start_motor();
  }
  //Stop button
  ImGui::SameLine();
  if(ImGui::Button("Stop motor", ImVec2(75, 0))){
    veloManager->lidar_stop_motor();
  }

  //Connection port
  int* velo_port = captureManager->get_capture_port();
  ImGui::SetNextItemWidth(item_width);
  if(ImGui::InputInt("Port", velo_port)){
    veloManager->lidar_stop_watcher();
  }

  //---------------------------
}
void GUI_Capture::velo_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Set RPM parameter
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "RPM");
    static int rot_freq_desired = 10;
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("##007", &rot_freq_desired, 5, 20, "%d Hz");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%d rpm", rot_freq_desired * 60);
    if(ImGui::Button("Set##1", ImVec2(item_width, 0))){
      int rot_rpm_desired = rot_freq_desired * 60;
      veloManager->lidar_set_rpm(rot_rpm_desired);
    }

    //Define Fiel Of View
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Field Of View");
    int fov_min = veloManager->get_fov_min();
    int fov_max = veloManager->get_fov_max();
    ImGui::SetNextItemWidth(item_width);
    ImGui::DragIntRange2("##008", &fov_min, &fov_max, 0, 0, 359, "%d°", "%d°");
    if(ImGui::Button("Set##2", ImVec2(item_width, 0))){
      veloManager->lidar_set_cameraFOV(fov_min, fov_max);
    }

    //---------------------------
    ImGui::Separator();
  }
}

//Scala subfunctions
void GUI_Capture::scala_state(){
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Scala");

  //---------------------------
}
void GUI_Capture::scala_file(){
  //---------------------------

  if(ImGui::Button("Load in dir", ImVec2(75, 0))){
    //scalaManager->loading("");
  }
  ImGui::SameLine();
  if(ImGui::Button("Load fast", ImVec2(75, 0))){
    //scalaManager->loading("/home/aether/Desktop/Velodium/media/scala");
  }

  //---------------------------
}
void GUI_Capture::scala_capture(){
  bool is_capturing = *scalaManager->get_is_scala_capturing();
  //---------------------------

  //Capturing button
  if(is_capturing == false){
    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start capture", ImVec2(157, 0))){
      captureManager->start_new_capture("scala");
    }
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop capture", ImVec2(157, 0))){
      captureManager->stop_capture();
    }
  }
  ImGui::PopStyleColor(1);

  //---------------------------
}
void GUI_Capture::scala_parameter(){
  if(ImGui::CollapsingHeader("Parameters##2")){
    //---------------------------

    //Connection port
    int* scala_port = scalaManager->get_capture_port();
    ImGui::SetNextItemWidth(item_width);
    ImGui::InputInt("Port", scala_port);

    //---------------------------
  }
}
