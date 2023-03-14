#include "GUI_Player.h"

#include "../Node_gui.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Player.h"
#include "../../Operation/Dynamic/Online.h"
#include "../../Operation/Dynamic/Visibility.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"

#include "image/IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Player::GUI_Player(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_operation* node_ope = node_gui->get_node_ope();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->node_interface = node_engine->get_node_interface();
  this->sceneManager = node_scene->get_sceneManager();
  this->playerManager = node_ope->get_playerManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->visibilityManager = node_ope->get_visibilityManager();

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

  //Display only the ieme collection
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
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  visibilityManager->compute_visibility_range_max(collection);
  int* visibility_range = visibilityManager->get_visibility_range();
  int visibility_range_max = visibilityManager->get_visibility_range_max();

  ImGui::SetNextItemWidth(140);
  if(ImGui::DragInt("Display##666", visibility_range, 1, 1, visibility_range_max)){
    if(collection != nullptr){
      playerManager->select_byObjectID(collection, collection->ID_obj_selected);
    }
  }

  //---------------------------
}
void GUI_Player::player_recording(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //Recording button
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(collection != nullptr){
      playerManager->player_save(collection);
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
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "On the fly");


  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_parameter(){
  Collection* collection = sceneManager->get_selected_collection();
  string* player_mode = playerManager->get_player_mode();
  //---------------------------

  //ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Parameter");

  //Restart to zero when arrive to the end of collection frames
  if(*player_mode == "player"){
    bool* with_restart = playerManager->get_with_restart();
    ImGui::Checkbox("Loop when end", with_restart);
  }

  //Setup collection point size
  if(collection != nullptr){
    /*int* point_size = &collection->draw_point_size;
    ImGui::SetNextItemWidth(50);
    ImGui::SliderInt("Point size", point_size, 1, 20);*/
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_button(){
  Collection* collection = sceneManager->get_selected_collection();
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
    this->button_player_play(collection);
    ImGui::SameLine();

    //Pause button
    this->button_player_pause(collection);
    ImGui::SameLine();

    //Stop button
    this->button_player_stop(collection);
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
    this->button_capture_play(collection);
    ImGui::SameLine();

    //Pause button
    this->button_capture_pause(collection);
    ImGui::SameLine();

    //Stop button
    this->button_capture_stop(collection);
    ImGui::SameLine();

    //Connection port
    this->button_capture_port();
  }

  //---------------------------
}
void GUI_Player::player_selection(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  if(collection != nullptr && collection->nb_obj > 2){
    Object_* object = collection->selected_obj;
    Object_* object_first = collection->get_obj(0);
    Object_* object_last = collection->get_obj(collection->list_obj.size()-1);
    int ID_obj = collection->ID_obj_selected;

    ImGui::SetNextItemWidth(140);
    if(ImGui::SliderInt("##666", &ID_obj, object_first->ID, object_last->ID)){
      if(collection != nullptr){
        playerManager->select_byObjectID(collection, ID_obj);
      }
    }
    ImGui::SameLine();
    if(object != nullptr && object->obj_type == "cloud"){
      float ts =  object->ts[0];
      ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", ts);
    }
  }

  //---------------------------
}
void GUI_Player::player_pause(){
  string* player_mode = playerManager->get_player_mode();
  //---------------------------

  if(*player_mode == "player"){
    playerManager->player_start_or_pause();
  }else if(*player_mode == "capture"){
    Capture* captureManager = node_interface->get_captureManager();
    bool* capture = captureManager->get_is_capturing();
    *capture = !*capture;
  }

  //---------------------------
}

//Specific button function
void GUI_Player::button_player_play(Collection* collection){
  bool is_playing = *playerManager->get_player_isrunning();
  //---------------------------

  if(is_playing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(collection != nullptr){
        playerManager->player_start();
      }
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(collection != nullptr){
        playerManager->player_pause();
      }
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_player_pause(Collection* collection){
  bool is_paused = *playerManager->get_player_ispaused();
  //---------------------------

  if(is_paused){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    if(collection != nullptr){
      playerManager->player_pause();
    }
  }
  if(is_paused){
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_player_stop(Collection* collection){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(collection != nullptr){
      playerManager->player_stop();
    }
  }

  //---------------------------
}

void GUI_Player::button_capture_play(Collection* collection){
  Capture* captureManager = node_interface->get_captureManager();
  bool* is_capturing = captureManager->get_is_capturing();
  //---------------------------

  if(*is_capturing){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    if(ImGui::Button(ICON_FA_PLAY "##36")){
      *is_capturing = false;
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      captureManager->start_new_capture("velodyne_vlp16");
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_capture_pause(Collection* collection){
  Capture* captureManager = node_interface->get_captureManager();
  bool is_capturing = *captureManager->get_is_capturing();
  bool is_finished = *captureManager->get_is_capture_finished();
  //---------------------------

  if(is_capturing == false && is_finished == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    bool* capture = captureManager->get_is_capturing();
    *capture = !*capture;
  }
  if(is_capturing == false && is_finished == false){
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Player::button_capture_stop(Collection* collection){
  Capture* captureManager = node_interface->get_captureManager();
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(collection != nullptr){
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
