#include "GUI_Dynamic.h"

#include "../Player_node.h"
#include "../Dynamic/Online.h"
#include "../Dynamic/Offline.h"
#include "../Dynamic/Followup.h"

#include "../../Module_node.h"
#include "../../Module_GUI.h"
#include "../../Interface/GUI/GUI_Interface.h"
#include "../../Interface/Interface_node.h"
#include "../../Interface/LiDAR/Capture.h"
#include "../../Interface/LiDAR/Velodyne/Velodyne.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Operation_GUI.h"
#include "../../../Operation/Color/Heatmap.h"
#include "../../../Operation/Transformation/Filter.h"
#include "../../../Operation/Color/GUI/GUI_Color.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Dynamic::GUI_Dynamic(GUI_module* gui_module){
  //---------------------------

  Engine_node* node_engine = gui_module->get_node_engine();
  Operation_node* node_ope = gui_module->get_node_ope();
  Module_node* node_module = gui_module->get_node_module();
  Player_node* node_player = node_module->get_node_player();
  Interface_node* node_interface = node_module->get_node_interface();
  GUI_operation* gui_operation = gui_module->get_gui_operation();

  this->gui_color = gui_operation->get_gui_color();
  this->gui_interface = gui_module->get_gui_interface();
  this->filterManager = node_ope->get_filterManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->onlineManager = node_player->get_onlineManager();
  this->offlineManager = node_player->get_offlineManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->followManager = node_player->get_followManager();
  this->captureManager = node_interface->get_captureManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Dynamic::~GUI_Dynamic(){}

//Main function
void GUI_Dynamic::design_dynamic(){
  //---------------------------

  this->player_run();
  this->parameter_online();
  this->parameter_offline();

  //---------------------------
}
void GUI_Dynamic::runtime_player_mouse(){
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

      offlineManager->select_bySubsetID(cloud, subset_selected_ID);
    }
  }

  //----------------------------
}

//Subfunctions
void GUI_Dynamic::player_run(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Timeline");
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = cloud->subset_selected;
  //---------------------------

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
      offlineManager->select_bySubsetID(cloud, cloud->ID_selected);
    }
  }

  //Recording
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(cloud != nullptr){
      offlineManager->player_save(cloud);
    }
  }
  ImGui::SameLine();
  //Dicrectory path selection & display
  if(ImGui::Button("...##23")){
    offlineManager->player_selectDirSave();
  }
  ImGui::SameLine();
  string saveas = *offlineManager->get_player_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_Dynamic::player_button(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Offline / Online mode
  string* player_mode = offlineManager->get_player_mode();
  if(*player_mode == "offline"){
    if (ImGui::Button("Offline##444")){
      *player_mode = "online";
    }
  }else{
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
    int freq = *offlineManager->get_frequency();
    ImGui::SetNextItemWidth(40);
    if(ImGui::SliderInt("Hz", &freq, 1, 25)){
      offlineManager->player_setFrequency(freq);
    }
  }
  //Online mode
  else{
    this->button_online_play(cloud);
    ImGui::SameLine();

    //Pause button
    this->button_online_pause(cloud);
    ImGui::SameLine();

    //Stop button
    this->button_online_stop(cloud);

    //Connection port
    Velodyne* veloManager = captureManager->get_veloManager();
    int* velo_port = veloManager->get_capture_port();
    ImGui::SetNextItemWidth(75);
    if(ImGui::InputInt("##555", velo_port)){
      veloManager->lidar_stop_watcher();
    }
  }

  //---------------------------
}
void GUI_Dynamic::player_selection(){
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
        offlineManager->select_bySubsetID(cloud, subset_selected_ID);
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
void GUI_Dynamic::parameter_offline(){
  if(ImGui::CollapsingHeader("Parameter - offline")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = cloud->subset_selected;
    //---------------------------

    //Restart to zero when arrive to the end of cloud frames
    bool* with_restart = offlineManager->get_with_restart();
    ImGui::Checkbox("Loop when end", with_restart);

    //Filter all cloud subset with cylinder cleaner
    if (ImGui::Button("Cylinder cleaning", ImVec2(120,0))){
      if(cloud != nullptr){
        filterManager->filter_cloud_cylinder(cloud);
      }
    }

    //Display all cloud frames
    if(ImGui::Button("All frame visible", ImVec2(120,0))){
      if(cloud != nullptr){
        for(int i=0; i<cloud->nb_subset; i++){
          Subset* subset = sceneManager->get_subset(cloud, i);
          subset->visibility = true;
        }
      }
    }

    //Setup cloud point size
    if(cloud != nullptr){
      int* point_size = &cloud->point_size;
      ImGui::SliderInt("Point size", point_size, 1, 20);
    }

    //Set or not heatmap for the entire cloud
    if(cloud != nullptr){
      bool heatmap = cloud->heatmap;
      if(ImGui::Checkbox("Heatmap", &heatmap)){
        heatmapManager->make_cloud_heatmap(cloud);
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Dynamic::parameter_online(){
  if(ImGui::CollapsingHeader("Parameter - online")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = cloud->subset_selected;
    //---------------------------

    //SLAM activated at each frame
    bool* with_slam = onlineManager->get_with_slam();
    ImGui::Checkbox("SLAM", with_slam);

    //Camera auto displacement
    bool* with_camera_follow = followManager->get_with_camera_follow();
    ImGui::Checkbox("Camera follow up", with_camera_follow);

    //Cylinder cleaning filter
    bool* cylinderFilter = onlineManager->get_with_cylinder_filter();
    ImGui::Checkbox("Cylinder cleaning", cylinderFilter);
    if(*cylinderFilter){
      float* r_min = filterManager->get_cyl_r_min();
      float* r_max = filterManager->get_cyl_r_max();
      float* z_min = filterManager->get_cyl_z_min();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("z min", z_min, 0.1f, 1.0f, "%.2f");
    }

    //Camera follow absolute position
    if(*with_camera_follow){
      bool* with_camera_absolute = followManager->get_with_camera_absolute();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width);
      ImGui::Checkbox("Absolute positionning", with_camera_absolute);
    }

    //GUI interface parameters
    gui_interface->parameter_dynamic();

    //Colorization
    gui_color->colorization_choice();

    //---------------------------
    ImGui::Separator();
  }
}

void GUI_Dynamic::button_offline_play(Cloud* cloud){
  bool is_playing = *offlineManager->get_player_isrunning();
  //---------------------------

  if(is_playing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(cloud != nullptr){
        offlineManager->player_start();
      }
    }
    ImGui::PopStyleColor(1);
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
    if (ImGui::Button(ICON_FA_PLAY "##36")){
      if(cloud != nullptr){
        offlineManager->player_pause();
      }
    }
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Dynamic::button_offline_pause(Cloud* cloud){
  bool is_paused = *offlineManager->get_player_ispaused();
  //---------------------------

  if(is_paused){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    if(cloud != nullptr){
      offlineManager->player_pause();
    }
  }
  if(is_paused){
    ImGui::PopStyleColor(1);
  }

  //---------------------------
}
void GUI_Dynamic::button_offline_stop(Cloud* cloud){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      offlineManager->player_stop();
    }
  }

  //---------------------------
}

void GUI_Dynamic::button_online_play(Cloud* cloud){
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
void GUI_Dynamic::button_online_pause(Cloud* cloud){
  bool is_capturing = *captureManager->get_is_capturing();
  //---------------------------

  if(is_capturing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    bool* capture = captureManager->get_is_capturing();
    *capture = !*capture;
  }
  if(is_capturing == false){
    ImGui::PopStyleColor(1);
  }



  //---------------------------
}
void GUI_Dynamic::button_online_stop(Cloud* cloud){
  //---------------------------

  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      captureManager->stop_capture();
    }
  }

  //---------------------------
}
