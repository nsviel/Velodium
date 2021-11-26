#include "GUI_Velodyne.h"

#include "Velodyne.h"

#include "../../../extern/imgui/imgui.h"



//Constructor / Destructor
GUI_velodyne::GUI_velodyne(){
  //---------------------------

  this->veloManager = new Velodyne();

  //---------------------------
}
GUI_velodyne::~GUI_velodyne(){}

//Main function
void GUI_velodyne::design_Velodyne(){
  //---------------------------

  this->State();
  this->Capture();
  this->Recording();
  this->Parameters();

  //---------------------------
}

//Subfunctions
void GUI_velodyne::State(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "State");
  //---------------------------

  //Start / Stop capturing
  bool* is_rotating = veloManager->get_is_rotating();
  if(*is_rotating == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Start##1", ImVec2(150,0))){
      veloManager->lidar_start();
    }
    ImGui::PopStyleColor(1);
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop##1", ImVec2(150,0))){

      //If the LiDAR is running, stop it
      veloManager->lidar_stop();
    }
    ImGui::PopStyleColor(1);
  }

  //Motor state
  int rot_freq = veloManager->get_rot_freq();
  int rot_rpm = veloManager->get_rot_rpm();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d Hz", rot_freq);
  ImGui::SameLine();
  ImGui::Text(" | ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.4f,1.0f), "%d rpm", rot_rpm);

  //Connected state
  bool* is_connected = veloManager->get_is_connected();
  ImGui::Text("Connected: ");
  ImGui::SameLine();
  if(*is_connected){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", *is_connected?"ON":"OFF");
  }else{
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", *is_connected?"ON":"OFF");
  }

  //Capture state
  ImGui::Text("Rotation: ");
  ImGui::SameLine();
  if(*is_rotating){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", *is_rotating?"ON":"OFF");
  }else{
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", *is_rotating?"ON":"OFF");
  }

  //Capture state
  bool* is_capturing = veloManager->get_is_capturing();
  ImGui::Text("Capture: ");
  ImGui::SameLine();
  if(*is_capturing){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", *is_capturing?"ON":"OFF");
  }else{
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", *is_capturing?"ON":"OFF");
  }

  //Capture state
  bool* is_recording = veloManager->get_is_recording();
  ImGui::Text("Recording: ");
  ImGui::SameLine();
  if(*is_recording){
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", *is_recording?"ON":"OFF");
  }else{
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%s", *is_recording?"ON":"OFF");
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_velodyne::Capture(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Capture data");
  //---------------------------

  //Start / Stop capturing
  bool* is_capturing = veloManager->get_is_capturing();
  bool* is_rotating = veloManager->get_is_rotating();
  if(*is_capturing == false){

    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Capture", ImVec2(150,0))){

      if(*is_rotating == false){
        console.AddLog("[error] LiDAR is not started");
      }else{
        *is_capturing = true;

        veloManager->lidar_startNewCapture();
        veloManager->run_capture();

        console.AddLog("Data capture ON");
      }

    }
    ImGui::PopStyleColor(1);

  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop##2", ImVec2(150,0))){
      *is_capturing = false;
      console.AddLog("Data capture OFF");
    }
    ImGui::PopStyleColor(1);

  }
  if(*is_capturing){
    //Read udp packets
    veloManager->onrun_ope();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_velodyne::Recording(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Recording");
  //---------------------------

  bool* is_capturing = veloManager->get_is_capturing();
  bool* is_rotating = veloManager->get_is_rotating();
  bool* is_recording = veloManager->get_is_recording();

  if(*is_recording == false){

    //Start button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if(ImGui::Button("Recording", ImVec2(150,0))){

      if(*is_rotating && *is_capturing){
        console.AddLog("[sucess] LiDAR recording...");
        *is_recording = true;
      }else{
        console.AddLog("[error] LiDAR is not recording");
        *is_recording = false;
      }

    }
    ImGui::PopStyleColor(1);
  }else{
    //Stop button
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if(ImGui::Button("Stop##3", ImVec2(150,0))){

      *is_recording = false;
      console.AddLog("Data recording OFF");

    }
    ImGui::PopStyleColor(1);
  }

  //Recording options -> number of frame
  bool* record_nb_frame = veloManager->get_is_record_n_frame();
  ImGui::Checkbox("##667", record_nb_frame);
  ImGui::SameLine();
  int* nb_frame = veloManager->get_record_n_frame_max();
  ImGui::SetNextItemWidth(75);
  ImGui::SliderInt("Number of frames", nb_frame, 1, 10000);

  //Recording options -> time interval
  bool* record_t_frame = veloManager->get_is_record_t_frame();
  ImGui::Checkbox("##668", record_t_frame);
  ImGui::SameLine();
  float* t_frame = veloManager->get_record_t_frame_max();
  ImGui::SetNextItemWidth(75);
  ImGui::SliderFloat("Time interval [s]", t_frame, 0.0f, 1000.0f);

  //Recording path
  if(ImGui::Button("...##24")){
    veloManager->recording_selectDirSave();
  }
  ImGui::SameLine();
  string saveas = *veloManager->get_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_velodyne::Parameters(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Set RPM parameter
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "RPM");

    // Define rpm parameter
    static int rot_freq_desired = 10;
    ImGui::SliderInt("##007", &rot_freq_desired, 5, 20, "%d Hz");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "%d rpm", rot_freq_desired * 60);

    if(ImGui::Button("Set##1", ImVec2(150,0))){
      int rot_rpm_desired = rot_freq_desired * 60;
      veloManager->lidar_set_rpm(rot_rpm_desired);
    }

    //---------------------------

    //Define Fiel Of View
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Field Of View");

    int fov_min = veloManager->get_fov_min();
    int fov_max = veloManager->get_fov_max();
    ImGui::DragIntRange2("##008", &fov_min, &fov_max, 0, 0, 359, "%d°", "%d°");
    if(ImGui::Button("Set min", ImVec2(150,0))){
      veloManager->lidar_set_fov_min(fov_min);
    }
    if(ImGui::Button("Set max", ImVec2(150,0))){
      veloManager->lidar_set_fov_max(fov_max);
    }

    //---------------------------
    ImGui::Separator();
  }
}
