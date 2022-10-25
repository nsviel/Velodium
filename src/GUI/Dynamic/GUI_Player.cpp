#include "GUI_Player.h"

#include "../Node_gui.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Player.h"
#include "../../Operation/Dynamic/Online.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"

#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Player::GUI_Player(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_operation* node_ope = node_gui->get_node_ope();

  this->node_interface = node_engine->get_node_interface();
  this->sceneManager = node_engine->get_sceneManager();
  this->playerManager = node_ope->get_playerManager();
  this->onlineManager = node_ope->get_onlineManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::design_player(){
  if(ImGui::BeginTabItem("Player")){
    //---------------------------

    this->design_run();
    this->player_onthefly();
    this->player_parameter();

    //---------------------------
    ImGui::EndTabItem();
  }
}
void GUI_Player::design_run(){
  //---------------------------

  //Display set of player buttons
  this->player_button();

  //Display only the ieme cloud
  this->player_selection();

  //Range of displayed frames
  this->player_visibility();

  //Recording
  this->player_recording();

  //---------------------------
  ImGui::Separator();
}

//Player action
void GUI_Player::player_visibility(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  int visible_range = onlineManager->get_visibility_range();
  int visible_range_max = onlineManager->get_visibility_range_max();
  ImGui::SetNextItemWidth(140);
  if(ImGui::DragInt("Display##666", &visible_range, 1, 1, visible_range_max)){
    onlineManager->set_visibility_range(visible_range);
    if(cloud != nullptr){
      playerManager->select_bySubsetID(cloud, cloud->ID_selected);
    }
  }

  //---------------------------
}
void GUI_Player::player_recording(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Recording button
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(cloud != nullptr){
      playerManager->player_save(cloud);
    }
  }

  //Dicrectory path selection & display
  ImGui::SameLine();
  if(ImGui::Button("...##23")){
    playerManager->player_selectDirSave();
  }

  ImGui::SameLine();
  string saveas = *playerManager->get_player_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
}
void GUI_Player::player_onthefly(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "On the fly");


  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_parameter(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  string* player_mode = playerManager->get_player_mode();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Parameter");

  //Restart to zero when arrive to the end of cloud frames
  if(*player_mode == "player"){
    bool* with_restart = playerManager->get_with_restart();
    ImGui::Checkbox("Loop when end", with_restart);
  }

  //Setup cloud point size
  if(cloud != nullptr){
    int* point_size = &cloud->point_size;
    ImGui::SetNextItemWidth(50);
    ImGui::SliderInt("Point size", point_size, 1, 20);
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_button(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  string* player_mode = playerManager->get_player_mode();
  //---------------------------

  //Player / Capture mode
  ImGui::Text("Mode");
  ImGui::SameLine();
  if(*player_mode == "player"){
    if (ImGui::Button("Player##444")){
      *player_mode = "capture";
    }
  }else if(*player_mode == "capture"){
    if (ImGui::Button("Capture##445")){
      *player_mode = "player";
    }
  }

  //Player mode
  if(*player_mode == "player"){
    //Play button
    this->button_player_play(cloud);
    ImGui::SameLine();

    //Pause button
    this->button_player_pause(cloud);
    ImGui::SameLine();

    //Stop button
    this->button_player_stop(cloud);
    ImGui::SameLine();

    //Frequency choice
    int freq = *playerManager->get_frequency();
    ImGui::SetNextItemWidth(40);
    if(ImGui::SliderInt("Hz", &freq, 1, 25)){
      playerManager->player_setFrequency(freq);
    }
  }
  //Online mode
  else if(*player_mode == "capture"){
    this->button_capture_play(cloud);
    ImGui::SameLine();

    //Pause button
    this->button_capture_pause(cloud);
    ImGui::SameLine();

    //Stop button
    this->button_capture_stop(cloud);
    ImGui::SameLine();

    //Connection port
    this->button_capture_port();
  }

  //---------------------------
}
void GUI_Player::player_selection(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(cloud != nullptr && cloud->nb_subset > 2){
    Subset* subset = cloud->subset_selected;
    Subset* subset_first = sceneManager->get_subset(cloud, 0);
    Subset* subset_last = sceneManager->get_subset(cloud, cloud->nb_subset-1);
    int subset_selected_ID = cloud->ID_selected;

    ImGui::SetNextItemWidth(140);
    if(ImGui::SliderInt("##666", &subset_selected_ID, subset_first->ID, subset_last->ID)){
      if(cloud != nullptr){
        playerManager->select_bySubsetID(cloud, subset_selected_ID);
      }
    }
    ImGui::SameLine();
    if(subset != nullptr){
      float ts =  subset->ts[0];
      ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", ts);
    }
  }

  //---------------------------
}

//Specific button function
void GUI_Player::button_player_play(Cloud* cloud){
  bool is_playing = *playerManager->get_player_isrunning();
  //---------------------------

  if(is_playing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(cloud != nullptr){
        playerManager->player_start();
      }
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(cloud != nullptr){
        playerManager->player_pause();
      }
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_player_pause(Cloud* cloud){
  bool is_paused = *playerManager->get_player_ispaused();
  //---------------------------

  if(is_paused){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    if(cloud != nullptr){
      playerManager->player_pause();
    }
  }
  if(is_paused){
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_player_stop(Cloud* cloud){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      playerManager->player_stop();
    }
  }

  //---------------------------
}

void GUI_Player::button_capture_play(Cloud* cloud){
  Capture* captureManager = node_interface->get_captureManager();
  bool* is_capturing = captureManager->get_is_capturing();
  //---------------------------

  if(*is_capturing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      captureManager->start_new_capture("velodyne_vlp16");
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      *is_capturing = false;
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_capture_pause(Cloud* cloud){
  Capture* captureManager = node_interface->get_captureManager();
  bool is_capturing = *captureManager->get_is_capturing();
  //---------------------------

  if(is_capturing){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    bool* capture = captureManager->get_is_capturing();
    *capture = !*capture;
  }
  if(is_capturing){
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_capture_stop(Cloud* cloud){
  Capture* captureManager = node_interface->get_captureManager();
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      captureManager->stop_capture();
    }
  }

  //---------------------------
}
void GUI_Player::button_capture_port(){
  Capture* captureManager = node_interface->get_captureManager();
  //---------------------------

  int* capture_port = captureManager->get_capture_port();
  ImGui::SetNextItemWidth(75);
  if(ImGui::InputInt("##555", capture_port)){
    captureManager->stop_capture();
  }

  //---------------------------
}
