#include "GUI_CloudPlayer.h"

#include "Online.h"
#include "CloudPlayer.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Scene.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../../extern/imgui/imgui.h"
#include "../../../extern/IconsFontAwesome5.h"


//Constructor / Destructor
GUI_CloudPlayer::GUI_CloudPlayer(Engine* engineManager){
  //---------------------------

  this->filterManager = engineManager->get_filterManager();
  this->playerManager = new CloudPlayer();
  this->heatmapManager = new Heatmap();
  this->sceneManager = new Scene();
  this->onlineManager = new Online(engineManager->get_CameraManager());

  //---------------------------
}
GUI_CloudPlayer::~GUI_CloudPlayer(){}

//Main function
void GUI_CloudPlayer::design_CloudPlayer(){
  //---------------------------

  this->playCloud();
  this->parameter_offline();
  this->parameter_online();

  //---------------------------
}

//Subfunctions
void GUI_CloudPlayer::playCloud(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Timeline");
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  //Play / Stop frame display
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if (ImGui::Button(ICON_FA_PLAY "##36")){
    if(cloud != nullptr){
      playerManager->playCloud_start();
    }
  }
  ImGui::PopStyleColor(1);
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    if(cloud != nullptr){
      playerManager->playCloud_pause();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      playerManager->playCloud_stop();
    }
  }
  ImGui::SameLine();
  int freq = *playerManager->get_frequency();
  ImGui::SetNextItemWidth(40);
  if(ImGui::SliderInt("Frequency", &freq, 1, 25)){
    playerManager->play_setFrequency(freq);
  }

  //Display only the ieme cloud
  this->subset_selection_bar();

  //Range of displayed frames
  int* subset_selected = playerManager->get_frame_ID();
  int* frame_max_nb = playerManager->get_frame_max_nb();
  int* frame_range = playerManager->get_frame_display_range();
  ImGui::SetNextItemWidth(140);
  if(ImGui::DragInt("Displayed frames", frame_range, 1, 0, *frame_max_nb)){
    if(cloud != nullptr){
      playerManager->select_byFrameID(cloud, *subset_selected);
    }
  }

  //Recording
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(cloud != nullptr){
      playerManager->playCloud_save(cloud);
    }
  }
  ImGui::SameLine();
  //Dicrectory path selection & display
  if(ImGui::Button("...##23")){
    playerManager->playCloud_selectDirSave();
  }
  ImGui::SameLine();
  string saveas = *playerManager->get_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_CloudPlayer::playCloud_byMouseWheel(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel - rolling stone
  if(io.MouseWheel && io.MouseDownDuration[1] == -1 && !io.WantCaptureMouse){
    if(cloud != nullptr){
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
void GUI_CloudPlayer::subset_selection_bar(){
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
void GUI_CloudPlayer::parameter_offline(){
  if(ImGui::CollapsingHeader("Parameters")){
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
void GUI_CloudPlayer::parameter_online(){
  if(ImGui::CollapsingHeader("Online parameters")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    bool* with_slam = onlineManager->get_with_slam();
    ImGui::Checkbox("SLAM each frame", with_slam);

    bool* cameraRoot = onlineManager->get_with_camera_follow();
    ImGui::Checkbox("Camera follow up", cameraRoot);

    bool* cylinderFilter = onlineManager->get_with_cylinder_filter();
    ImGui::Checkbox("Cylinder cleaning", cylinderFilter);

    bool* withHeatmap = onlineManager->get_with_heatmap();
    ImGui::Checkbox("Heatmap", withHeatmap);

    //---------------------------
    ImGui::Separator();
  }
}
