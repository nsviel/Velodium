#include "GUI_Player.h"

#include "../Player_online.h"
#include "../Player_cloud.h"
#include "../../Module_node.h"
#include "../../Obstacle/Interfacing.h"

#include "../../../GUI/GUI_node.h"
#include "../../../Engine/Scene.h"
#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Functions/Heatmap.h"
#include "../../../Operation/Transformation/Filter.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Player::GUI_Player(GUI_node* node_gui){
  //---------------------------

  Operation_node* node_ope = node_gui->get_node_ope();
  Module_node* node_module = node_gui->get_node_module();

  this->filterManager = node_ope->get_filterManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->onlineManager = node_module->get_onlineManager();
  this->playerManager = node_module->get_playerManager();
  this->ioManager = node_module->get_ioManager();
  this->sceneManager = new Scene();

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
void GUI_Player::update(){
  //---------------------------

  onlineManager->update_configuration();

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
  int* player_subset_range = onlineManager->get_visibility_range();
  int nb_subset_max = *player_subset_range;
  if(cloud != nullptr){
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
void GUI_Player::player_mouse(){
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
void GUI_Player::player_selection(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(cloud != nullptr && cloud->nb_subset > 2){
    Subset* subset = sceneManager->get_subset_selected();
    Subset* subset_first = sceneManager->get_subset(cloud, 0);
    Subset* subset_last = sceneManager->get_subset(cloud, cloud->nb_subset-1);
    int subset_selected_ID = cloud->ID_selected;
    float ts =  subset->ts[0];

    ImGui::SetNextItemWidth(140);
    if(ImGui::SliderInt("##666", &subset_selected_ID, subset_first->ID, subset_last->ID)){
      if(cloud != nullptr){
        playerManager->select_bySubsetID(cloud, subset_selected_ID);
      }
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", ts);

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

    if(ImGui::Button("All frame visible", ImVec2(120,0))){
      if(cloud != nullptr){
        for(int i=0; i<cloud->nb_subset; i++){
          Subset* subset = sceneManager->get_subset(cloud, i);
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
    ImGui::Columns(2);
    //---------------------------

    bool* with_slam = onlineManager->get_with_slam();
    ImGui::Checkbox("SLAM each frame", with_slam);
    ImGui::NextColumn();

    ImGui::NextColumn();
    bool* with_camera_follow = onlineManager->get_with_camera_follow();
    ImGui::Checkbox("Camera follow up", with_camera_follow);
    ImGui::NextColumn();

    ImGui::NextColumn();
    bool* with_remove_lastSubset = onlineManager->get_with_remove_lastSubset();
    ImGui::Checkbox("Remove last subset", with_remove_lastSubset);
    ImGui::NextColumn();

    ImGui::NextColumn();
    bool* with_save_frame = onlineManager->get_with_save_frame();
    ImGui::Checkbox("Save frame", with_save_frame);
    ImGui::NextColumn();
    static bool with_unlimit_saving = false;
    if(*with_save_frame && with_unlimit_saving == false){
      int* save_frame_max = ioManager->get_save_frame_max();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb frame", save_frame_max);
    }
    ImGui::NextColumn();
    ImGui::Checkbox("Save unlimited frame", &with_unlimit_saving);
    if(with_unlimit_saving){
      int* save_frame_max = ioManager->get_save_frame_max();
      *save_frame_max = 500000000;
    }
    ImGui::NextColumn();

    ImGui::NextColumn();
    bool* with_save_image = onlineManager->get_with_save_image();
    ImGui::Checkbox("Save image", with_save_image);
    ImGui::NextColumn();
    if(*with_save_image){
      int* save_image_max = ioManager->get_save_image_max();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb image", save_image_max);
    }

    /*if(*with_remove_lastSubset){
      int* nb_subset_max = onlineManager->get_nb_subset_max();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb cloud subset", nb_subset_max);
    }*/
    ImGui::NextColumn();

    //Colorization
    int colorization;
    bool* with_heatmap = onlineManager->get_with_heatmap();
    if(*with_heatmap){
      colorization = 0;
    }else{
      colorization = 1;
    }
    if(ImGui::RadioButton("Heatmap", &colorization, 0)){
      *onlineManager->get_with_heatmap() = true;
      *onlineManager->get_with_unicolor() = false;
    }
    ImGui::NextColumn();

    //Option: heatmap with relative height
    bool* heatmap_rltHeight = onlineManager->get_with_heatmap_rltHeight();
    if(colorization == 0){
      ImGui::Checkbox("Heatmap relative height", heatmap_rltHeight);
    }

    ImGui::NextColumn();
    if(ImGui::RadioButton("Unicolor", &colorization, 1)){
      *onlineManager->get_with_heatmap() = false;
      *onlineManager->get_with_unicolor() = true;
    }
    ImGui::NextColumn();

    if(colorization == 0 && *heatmap_rltHeight){
      vec2* height_range = onlineManager->get_heatmap_height_range();
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("Z min", &height_range->x, 0.1f, 1.0f, "%.2f");
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("Z max", &height_range->y, 0.1f, 1.0f, "%.2f");
    }

    //Cylinder cleaning filter
    ImGui::NextColumn();
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
