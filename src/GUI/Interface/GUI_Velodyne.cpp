#include "GUI_Velodyne.h"
#include "GUI_Network.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"
#include "../../Interface/Capture/Scala/Scala.h"
#include "../../Interface/Capture/Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Velodyne::GUI_Velodyne(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->captureManager = node_interface->get_captureManager();
  this->veloManager = captureManager->get_veloManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Velodyne::~GUI_Velodyne(){}

//Main function
void GUI_Velodyne::design_Velodyne(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Velodyne");
  //---------------------------

  this->velo_capture();
  this->velo_statistique();
  this->velo_parameter();

  //---------------------------
}

//Velodyne subfunctions
void GUI_Velodyne::velo_state(){
  //---------------------------

  //Watchers Velodyne
  bool is_velodyne_watcher = *veloManager->get_run_capture();
  ImGui::Text("Watcher - Velodyne");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", is_velodyne_watcher ? "ON" : "OFF");

  //---------------------------
}
void GUI_Velodyne::velo_statistique(){
  //---------------------------

  //LiDAR IP
  string* ip = veloManager->get_lidar_ip();
  static char str_n[256];
  strcpy(str_n, (*ip).c_str());
  if(ImGui::InputText("IP##444", str_n, IM_ARRAYSIZE(str_n), ImGuiInputTextFlags_EnterReturnsTrue)){
    *ip = str_n;
  }

  //Capture time
  ImGui::Text("Packet ");
  ImGui::SameLine();
  float time_packet = veloManager->get_time_packet();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%.2f", time_packet);
  ImGui::SameLine();
  ImGui::Text(" ms");
  ImGui::Text("Frame");
  ImGui::SameLine();
  float time_frame = veloManager->get_time_frame();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%.2f", time_frame);
  ImGui::SameLine();
  ImGui::Text(" ms");

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
  ImGui::Separator();
}
void GUI_Velodyne::velo_capture(){
  bool is_rotating = *veloManager->get_is_rotating();
  bool is_capturing = *veloManager->get_run_capture();
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

  //Start LIDAR motor
  if(is_capturing && is_rotating){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    ImGui::Button("##333", ImVec2(46, 0));
    ImGui::PopStyleColor(1);
  }else{
    if(ImGui::Button("Start", ImVec2(46, 0))){
      veloManager->lidar_start_motor();
    }
  }

  //Stop LIDAR motor
  ImGui::SameLine();
  if(is_capturing && is_rotating == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(150, 40, 40, 255));
    ImGui::Button("##334", ImVec2(46, 0));
    ImGui::PopStyleColor(1);
  }else{
    if(ImGui::Button("Stop", ImVec2(46, 0))){
      veloManager->lidar_stop_motor();
    }
  }
  ImGui::SameLine();
  ImGui::Text("Motor");

  //Connection port
  int* port = captureManager->get_capture_port();
  ImGui::SetNextItemWidth(item_width);
  if(ImGui::InputInt("Port", port)){
    captureManager->stop_capture();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Velodyne::velo_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Set frame ratio
    int ratio_frame = captureManager->get_ratio_frame();
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::SliderInt("Frame ratio", &ratio_frame, 1, 20)){
      captureManager->set_ratio_frame(ratio_frame);
    }

    //Set RPM parameter
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "LiDAR RPM");
    static int rot_freq_desired = 10;
    ImGui::SetNextItemWidth(item_width);
    ImGui::SliderInt("##007", &rot_freq_desired, 5, 20, "%d Hz");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d rpm", rot_freq_desired * 60);
    if(ImGui::Button("Set##1", ImVec2(item_width, 0))){
      int rot_rpm_desired = rot_freq_desired * 60;
      veloManager->lidar_set_rpm(rot_rpm_desired);
    }

    //Define Fiel Of View
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "LiDAR Field Of View");
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
