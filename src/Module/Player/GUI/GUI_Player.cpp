#include "GUI_Player.h"

#include "../Player_online.h"
#include "../Player_cloud.h"

#include "../../../Engine/Engine.h"
#include "../../../Engine/Scene.h"
#include "../../../Operation/Functions/Heatmap.h"
#include "../../../Operation/Transformation/Filter.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Player::GUI_Player(Engine* engineManager){
  //---------------------------

  this->filterManager = engineManager->get_filterManager();
  this->heatmapManager = new Heatmap();
  this->sceneManager = new Scene();
  this->onlineManager = new Player_online(engineManager);
  this->playerManager = new Player_cloud(onlineManager);

  //---------------------------
}
GUI_Player::~GUI_Player(){}

//Main function
void GUI_Player::design_player_cloud(){
  //---------------------------

  this->player_run();
  this->parameter_online();
  this->parameter_offline();

  //---------------------------
}
void GUI_Player::design_player_online(){
  //---------------------------


  //---------------------------
}

//Subfunctions
void GUI_Player::player_run(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Timeline");
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  //Play / Stop frame display
  playerManager->player_runtime();
  bool is_playing = *playerManager->get_player_isrunning();
  if(is_playing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PLAY "##36")){
    if(cloud != nullptr){
      playerManager->player_start();
    }
  }
  ImGui::PopStyleColor(1);
  ImGui::SameLine();
  bool is_paused = *playerManager->get_player_ispaused();
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
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      playerManager->player_stop();
    }
  }
  ImGui::SameLine();
  int freq = *playerManager->get_frequency();
  ImGui::SetNextItemWidth(40);
  if(ImGui::SliderInt("Frequency", &freq, 1, 25)){
    playerManager->player_setFrequency(freq);
  }

  //Display only the ieme cloud
  this->player_selection();

  //Range of displayed frames
  int* subset_selected = playerManager->get_frame_ID();
  int* frame_max_nb = playerManager->get_frame_max_nb();
  int* frame_range = playerManager->get_player_frame_range();
  ImGui::SetNextItemWidth(140);
  if(ImGui::DragInt("Displayed frames", frame_range, 1, 0, *frame_max_nb)){
    if(cloud != nullptr){
      playerManager->select_byFrameID(cloud, *subset_selected);
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
  string saveas = *playerManager->get_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_Player::player_mouse(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel - rolling stone
  if(io.MouseWheel && io.MouseDownDuration[1] == -1 && !io.WantCaptureMouse){
    if(cloud != nullptr){
      playerManager->update_frame_ID(cloud);
      int subset_selected = *playerManager->get_frame_ID();

      if(io.MouseWheel > 0){
        subset_selected++;
      }else{
        subset_selected--;
      }

      playerManager->select_byFrameID(cloud, subset_selected);
    }
  }

  //----------------------------
}
void GUI_Player::player_selection(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(cloud != nullptr){
    playerManager->update_frame_ID(cloud);
    int* subset_selected = playerManager->get_frame_ID();
    int* frame_max_ID = playerManager->get_frame_max_ID();

    ImGui::SetNextItemWidth(140);
    if(ImGui::SliderInt("##666", subset_selected, 0, *frame_max_ID)){
      if(cloud != nullptr){
        playerManager->select_byFrameID(cloud, *subset_selected);
      }
    }
    ImGui::SameLine();

    Subset* subset = sceneManager->get_subset_selected();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", subset->ts[0]);
  }

  //---------------------------
}
void GUI_Player::parameter_offline(){
  if(ImGui::CollapsingHeader("Offline params")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    bool* with_restart = playerManager->get_with_restart();
    ImGui::Checkbox("Loop when end", with_restart);

    if (ImGui::Button("Cylinder cleaning", ImVec2(120,0))){
      if(cloud != nullptr){
        filterManager->filter_cloud_cylinder(cloud);
      }
    }

    if (ImGui::Button("Supress first subset", ImVec2(120,0))){
      if(cloud != nullptr){
        playerManager->supress_firstSubset(cloud);
      }
    }

    if(ImGui::Button("All frame visible", ImVec2(120,0))){
      if(cloud != nullptr){
        for(int i=0; i<cloud->nb_subset; i++){
          Subset* subset = &cloud->subset[i];
          subset->visibility = true;
        }
      }
    }

    //Choice of the LiDAR model
    ImGui::SetNextItemWidth(75);
    static int lidar_model = 0;
    if (ImGui::Combo("##1", &lidar_model, "VLP-16\0HDL-32\0")){

    }

    if(cloud != nullptr){
      int* point_size = &cloud->point_size;
      ImGui::SliderInt("Point size", point_size, 1, 20);
    }

    //Set or not heatmap for the entire cloud
    static bool heatmap = false;
    if(ImGui::Checkbox("Heatmap", &heatmap)){
      if(cloud != nullptr){
        heatmapManager->set_Heatmap(cloud, heatmap);
      }
    }

    //Set heatmap range
    vec2 heatmap_range = *heatmapManager->get_heatmap_range();
    int min = (int) (heatmap_range.x * 255);
    int max = (int) (heatmap_range.y * 255);

    if(ImGui::DragIntRange2("Intensity", &min, &max, 1, 0, 255, "%d", "%d")){
      vec2* range = heatmapManager->get_heatmap_range();
      *range = vec2((float)min / 255, (float)max / 255);
      heatmapManager->set_Heatmap(cloud, heatmap);
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_Player::parameter_online(){
  bool* with_online = onlineManager->get_with_online();
  ImGui::Checkbox("Online processing", with_online);

  if(ImGui::CollapsingHeader("Online params")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    bool* with_slam = onlineManager->get_with_slam();
    ImGui::Checkbox("SLAM each frame", with_slam);

    bool* with_camera_follow = onlineManager->get_with_camera_follow();
    ImGui::Checkbox("Camera follow up", with_camera_follow);

    ImGui::Columns(2);
    bool* with_save_image = onlineManager->get_with_save_image();
    ImGui::Checkbox("Save image", with_save_image);

    //Dicrectory path selection & display
    ImGui::NextColumn();
    if(*with_save_image){
      if(ImGui::Button("...##26")){
        onlineManager->save_image_path();
      }
      ImGui::SameLine();
      string saveas = *onlineManager->get_save_image_path();
      ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());
    }
    ImGui::NextColumn();

    bool* withHeatmap = onlineManager->get_with_heatmap();
    ImGui::Checkbox("Heatmap", withHeatmap);

    bool* heatmap_rltHeight = onlineManager->get_with_heatmap_rltHeight();
    ImGui::NextColumn();
    if(*withHeatmap){
      ImGui::Checkbox("Heatmap relative height", heatmap_rltHeight);

      if(*heatmap_rltHeight){
        vec2* height_range = onlineManager->get_heatmap_height_range();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Z min", &height_range->x, 0.1f, 1.0f, "%.2f");
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("Z max", &height_range->y, 0.1f, 1.0f, "%.2f");
      }
    }
    ImGui::NextColumn();

    //Cylinder cleaning filter
    bool* cylinderFilter = onlineManager->get_with_cylinder_filter();
    ImGui::Checkbox("Cylinder cleaning", cylinderFilter);
    ImGui::NextColumn();
    if(*cylinderFilter){
      float* r_min = filterManager->get_cyl_r_min();
      float* r_max = filterManager->get_cyl_r_max();
      float* z_min = filterManager->get_cyl_z_min();
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("z min", z_min, 0.1f, 1.0f, "%.2f");
    }
    ImGui::NextColumn();

    ImGui::Columns(1);

    //---------------------------
    ImGui::Separator();
  }
}
