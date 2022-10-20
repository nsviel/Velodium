#include "GUI_Player.h"

#include "../Player.h"
#include "../Online.h"

#include "../../Node_operation.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Interface/Node_interface.h"
#include "../../../Interface/Capture/Capture.h"

#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Player::GUI_Player(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_interface* node_interface = node_engine->get_node_interface();

  this->sceneManager = node_engine->get_sceneManager();
  this->playerManager = node_ope->get_playerManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->captureManager = node_interface->get_captureManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::runtime_player_mouse(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel - rolling stone
  if(io.MouseWheel && io.MouseDownDuration[1] == -1 && !io.WantCaptureMouse){
    if(cloud != nullptr){
      int subset_selected_ID = cloud->ID_selected;

      if(io.MouseWheel > 0){
        subset_selected_ID++;
      }else{
        subset_selected_ID--;
      }

      playerManager->select_bySubsetID(cloud, subset_selected_ID);
    }
  }

  //----------------------------
}

//Subfunction
void GUI_Player::player_run(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Mode");
  ImGui::SameLine();

  //Display set of player buttons
  this->player_button();

  //Display only the ieme cloud
  this->player_selection();

  //Range of displayed frames
  int* player_subset_range = onlineManager->get_visibility_range();
  int nb_subset_max = *player_subset_range;
  if(cloud != nullptr && sceneManager->get_is_list_empty() == false){
    nb_subset_max = cloud->nb_subset - 1;
  }
  ImGui::SetNextItemWidth(140);
  if(ImGui::DragInt("Displayed frames", player_subset_range, 1, 1, nb_subset_max)){
    if(cloud != nullptr){
      playerManager->select_bySubsetID(cloud, cloud->ID_selected);
    }
  }

  //Recording
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(cloud != nullptr){
      playerManager->player_save(cloud);
    }
  }
  ImGui::SameLine();
  //Dicrectory path selection & display
  if(ImGui::Button("...##23")){
    playerManager->player_selectDirSave();
  }
  ImGui::SameLine();
  string saveas = *playerManager->get_player_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_button(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Offline / Online mode
  string* player_mode = playerManager->get_player_mode();
  if(*player_mode == "offline"){
    if (ImGui::Button("Offline##444")){
      *player_mode = "online";
    }
  }else if(*player_mode == "online"){
    if (ImGui::Button("Online##445")){
      *player_mode = "offline";
    }
  }

  //Offline mode
  if(*player_mode == "offline"){
    //Play button
    this->button_offline_play(cloud);
    ImGui::SameLine();

    //Pause button
    this->button_offline_pause(cloud);
    ImGui::SameLine();

    //Stop button
    this->button_offline_stop(cloud);
    ImGui::SameLine();

    //Frequency choice
    int freq = *playerManager->get_frequency();
    ImGui::SetNextItemWidth(40);
    if(ImGui::SliderInt("Hz", &freq, 1, 25)){
      playerManager->player_setFrequency(freq);
    }
  }
  //Online mode
  else if(*player_mode == "online"){
    this->button_online_play(cloud);
    ImGui::SameLine();

    //Pause button
    this->button_online_pause(cloud);
    ImGui::SameLine();

    //Stop button
    this->button_online_stop(cloud);
    ImGui::SameLine();

    //Connection port
    int* velo_port = captureManager->get_capture_port();
    ImGui::SetNextItemWidth(75);
    if(ImGui::InputInt("##555", velo_port)){
      captureManager->stop_capture();
    }
  }

  //---------------------------
}
void GUI_Player::player_selection(){
  Cloud* cloud = sceneManager->get_cloud_selected();
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
void GUI_Player::button_offline_play(Cloud* cloud){
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
void GUI_Player::button_offline_pause(Cloud* cloud){
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
void GUI_Player::button_offline_stop(Cloud* cloud){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      playerManager->player_stop();
    }
  }

  //---------------------------
}

void GUI_Player::button_online_play(Cloud* cloud){
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
void GUI_Player::button_online_pause(Cloud* cloud){
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
void GUI_Player::button_online_stop(Cloud* cloud){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      captureManager->stop_capture();
    }
  }

  //---------------------------
}
