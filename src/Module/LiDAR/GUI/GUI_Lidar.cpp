#include "GUI_Lidar.h"

#include "../Capture.h"
#include "../Scala/Scala.h"
#include "../Velodyne/Velodyne.h"


//Constructor / Destructor
GUI_Lidar::GUI_Lidar(){
  //---------------------------

  this->captureManager = new Capture();
  this->scalaManager = captureManager->get_scalaManager();
  this->veloManager = captureManager->get_veloManager();

  //---------------------------
}
GUI_Lidar::~GUI_Lidar(){}

//Main function
void GUI_Lidar::design_Scala(){
  if(ImGui::CollapsingHeader("Scala")){
    //---------------------------

    if(ImGui::Button("Load in directory", ImVec2(100,0))){
      scalaManager->loading("");
    }

    if(ImGui::Button("Load fast", ImVec2(100,0))){
      scalaManager->loading("/home/aither/Desktop/Velodium/media/scala");
    }

    //---------------------------
  }
}
void GUI_Lidar::design_Velodyne(){
  //---------------------------

  captureManager->runtime_capturing();
  this->velo_state();
  this->velo_capture();
  this->velo_parameter();

  //---------------------------
}

//Velodyne subfunctions
void GUI_Lidar::velo_state(){
  //---------------------------

  //Motor state
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "State");
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
void GUI_Lidar::velo_connection(){
  //---------------------------

  bool* is_connected = veloManager->get_is_connected();
  bool* is_rotating = veloManager->get_is_rotating();

  //Start LiDAR button
  if(*is_rotating == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start##1", ImVec2(150,0))){
      veloManager->lidar_start_motor();
    }
    ImGui::PopStyleColor(1);
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop LiDAR", ImVec2(150,0))){

      //If the LiDAR is running, stop it
      veloManager->lidar_stop_motor();
    }
    ImGui::PopStyleColor(1);
  }

  //Connected state
  ImGui::SameLine();
  if(*is_connected && *is_rotating){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", "ON");
  }else{
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", "OFF");
  }

  //---------------------------
}
void GUI_Lidar::velo_capture(){
  bool is_capturing = *veloManager->get_is_capturing();
  //---------------------------

  //Capturing button
  if(is_capturing == false){
    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Capture", ImVec2(150,0))){
      captureManager->start_new_capture();
    }
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", "ON");
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop capture", ImVec2(150,0))){
      captureManager->stop_capture();
    }
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", "OFF");
  }

  //---------------------------
}
void GUI_Lidar::velo_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Set RPM parameter
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "RPM");
    static int rot_freq_desired = 10;
    ImGui::SliderInt("##007", &rot_freq_desired, 5, 20, "%d Hz");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%d rpm", rot_freq_desired * 60);
    if(ImGui::Button("Set##1", ImVec2(145,0))){
      int rot_rpm_desired = rot_freq_desired * 60;
      veloManager->lidar_set_rpm(rot_rpm_desired);
    }

    //Define Fiel Of View
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Field Of View");
    int fov_min = veloManager->get_fov_min();
    int fov_max = veloManager->get_fov_max();
    ImGui::DragIntRange2("##008", &fov_min, &fov_max, 0, 0, 359, "%d°", "%d°");
    if(ImGui::Button("Set##2", ImVec2(145,0))){
      veloManager->lidar_set_cameraFOV(fov_min, fov_max);
    }

    //---------------------------
    ImGui::Separator();
  }
}
