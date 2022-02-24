#include "GUI_Dynamic.h"

#include "../../Player.h"
#include "../../Dynamic/Online.h"
#include "../../Dynamic/Offline.h"

#include "../../../Module_node.h"
#include "../../../Module_GUI.h"
#include "../../../Interface/Interface.h"
#include "../../../Interface/Component/Saving.h"

#include "../../../../Engine/Scene/Scene.h"
#include "../../../../Operation/Operation_node.h"
#include "../../../../Operation/Functions/Heatmap.h"
#include "../../../../Operation/Transformation/Filter.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Dynamic::GUI_Dynamic(GUI_module* node_gui){
  //---------------------------

  Operation_node* node_ope = node_gui->get_node_ope();
  Module_node* node_module = node_gui->get_node_module();
  Player* playerManager = node_module->get_playerManager();

  this->filterManager = node_ope->get_filterManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->onlineManager = playerManager->get_onlineManager();
  this->offlineManager = playerManager->get_offlineManager();
  this->ioManager = node_module->get_ioManager();
  this->sceneManager = new Scene();

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

//Subfunctions
void GUI_Dynamic::player_run(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Timeline");
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  //Display set of player buttons
  this->player_button();

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

  //Play button
  offlineManager->player_runtime();
  bool is_playing = *offlineManager->get_player_isrunning();
  if(is_playing == false){
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  }else{
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 133, 45, 255));
  }
  if (ImGui::Button(ICON_FA_PLAY "##36")){
    if(cloud != nullptr){
      offlineManager->player_start();
    }
  }
  ImGui::PopStyleColor(1);
  ImGui::SameLine();

  //Pause button
  bool is_paused = *offlineManager->get_player_ispaused();
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
  ImGui::SameLine();

  //Stop button
  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      offlineManager->player_stop();
    }
  }
  ImGui::SameLine();

  //Frequency choice
  int freq = *offlineManager->get_frequency();
  ImGui::SetNextItemWidth(40);
  if(ImGui::SliderInt("Hz", &freq, 1, 25)){
    offlineManager->player_setFrequency(freq);
  }

  //---------------------------
}
void GUI_Dynamic::player_selection(){
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
        offlineManager->select_bySubsetID(cloud, subset_selected_ID);
      }
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", ts);

  }

  //---------------------------
}
void GUI_Dynamic::parameter_offline(){
  if(ImGui::CollapsingHeader("Offline params")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    bool* with_restart = offlineManager->get_with_restart();
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
void GUI_Dynamic::parameter_online(){
  if(ImGui::CollapsingHeader("Parameter - online")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //SLAM activated at each frame
    bool* with_slam = onlineManager->get_with_slam();
    ImGui::Checkbox("SLAM", with_slam);

    //Camera auto displacement
    bool* with_camera_follow = onlineManager->get_with_camera_follow();
    ImGui::Checkbox("Camera follow up", with_camera_follow);

    //Remove last subset
    bool* with_remove_lastSubset = onlineManager->get_with_remove_lastSubset();
    ImGui::Checkbox("Remove last subset", with_remove_lastSubset);

    //Save frame in folder for AI module
    bool* with_save_frame = onlineManager->get_with_save_frame();
    ImGui::Checkbox("Save frame", with_save_frame);
    if(*with_save_frame){
      Saving* saveManager = ioManager->get_saveManager();
      int* save_frame_max = saveManager->get_save_frame_max();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb frame", save_frame_max);
    }

    //Option: unlimited frame saving
    static bool with_unlimit_saving = false;
    ImGui::Checkbox("Save unlimited frame", &with_unlimit_saving);
    if(with_unlimit_saving){
      Saving* saveManager = ioManager->get_saveManager();
      int* save_frame_max = saveManager->get_save_frame_max();
      *save_frame_max = 500000000;
    }

    //Save image for interfacing
    bool* with_save_image = onlineManager->get_with_save_image();
    ImGui::Checkbox("Save image", with_save_image);
    if(*with_save_image){
      Saving* saveManager = ioManager->get_saveManager();
      int* save_image_max = saveManager->get_save_image_max();
      ImGui::SetNextItemWidth(100);
      ImGui::InputInt("Nb image", save_image_max);
    }

    //Colorization
    int colorization;
    bool* with_heatmap = onlineManager->get_with_heatmap();
    if(*with_heatmap){colorization = 0;}else{colorization = 1;}
    if(ImGui::RadioButton("Heatmap", &colorization, 0)){
      *onlineManager->get_with_heatmap() = true;
      *onlineManager->get_with_unicolor() = false;
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("Unicolor", &colorization, 1)){
      *onlineManager->get_with_heatmap() = false;
      *onlineManager->get_with_unicolor() = true;
    }

    //Option: heatmap with relative height
    bool* heatmap_rltHeight = onlineManager->get_with_heatmap_rltHeight();
    if(colorization == 0){
      ImGui::Checkbox("Heatmap relative height", heatmap_rltHeight);
    }
    if(colorization == 0 && *heatmap_rltHeight){
      vec2* height_range = onlineManager->get_heatmap_height_range();
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("Z min", &height_range->x, 0.1f, 1.0f, "%.2f");
      ImGui::SetNextItemWidth(100);
      ImGui::InputFloat("Z max", &height_range->y, 0.1f, 1.0f, "%.2f");
    }

    //Cylinder cleaning filter
    bool* cylinderFilter = onlineManager->get_with_cylinder_filter();
    ImGui::Checkbox("Cylinder cleaning", cylinderFilter);
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

    //---------------------------
    ImGui::Separator();
  }
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
