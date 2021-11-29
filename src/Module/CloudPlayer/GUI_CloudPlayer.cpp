#include "GUI_CloudPlayer.h"

#include "CloudPlayer.h"

#include "../../Engine/Data/Database.h"
#include "../../Operation/Functions/Heatmap.h"

#include "../../../extern/imgui/imgui.h"

#include <thread>

extern struct Database database;



//Constructor / Destructor
GUI_CloudPlayer::GUI_CloudPlayer(){
  //---------------------------

  this->odoManager = new CloudPlayer();
  this->heatmapManager = new Heatmap();

  //---------------------------
}
GUI_CloudPlayer::~GUI_CloudPlayer(){}

//Main function
void GUI_CloudPlayer::design_CloudPlayer(){
  //---------------------------

  this->playCloud();
  this->playCloud_byMouseWheel();
  this->parameter();

  //---------------------------
}

//Subfunctions
void GUI_CloudPlayer::playCloud(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Timeline");
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  //Play / Stop frame display
  if (ImGui::Button(ICON_FA_PLAY "##36")){
    if(cloud != nullptr){
      odoManager->playCloud_start();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_PAUSE "##37")){
    if(cloud != nullptr){
      odoManager->playCloud_pause();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STOP "##37")){
    if(cloud != nullptr){
      odoManager->playCloud_stop();
    }
  }
  ImGui::SameLine();
  int freq = *odoManager->get_frequency();
  ImGui::SetNextItemWidth(40);
  if(ImGui::SliderInt("Frequency", &freq, 1, 20)){
    odoManager->play_setFrequency(freq);
  }

  //Display only the ieme cloud
  odoManager->update_frame_ID(cloud);
  int* subset_selected = odoManager->get_frame_ID();
  int* frame_max_ID = odoManager->get_frame_max_ID();
  bool* all_frame_visible = odoManager->get_all_frame_visible();
  if(ImGui::SliderInt("##666", subset_selected, 0, *frame_max_ID)){
    if(cloud != nullptr){
      *all_frame_visible = false;
      odoManager->select_byFrameID(cloud, *subset_selected);
    }
  }
  ImGui::SameLine();
  float* frame_ID_ts = odoManager->get_frame_ID_ts();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%.4f", *frame_ID_ts);

  //Range of displayed frames
  int* frame_max_nb = odoManager->get_frame_max_nb();
  int* frame_range = odoManager->get_frame_display_range();
  if(ImGui::DragInt("Displayed frames", frame_range, 1, 0, *frame_max_nb)){
    if(cloud != nullptr){
      odoManager->select_byFrameID(cloud, *subset_selected);
    }
  }

  //Recording
  if (ImGui::Button(ICON_FA_CIRCLE "##37")){
    if(cloud != nullptr){
      odoManager->playCloud_save(cloud);
    }
  }
  ImGui::SameLine();
  if(ImGui::Button("...##23")){
    odoManager->playCloud_selectDirSave();
  }
  ImGui::SameLine();
  string saveas = *odoManager->get_saveas();
  ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", saveas.c_str());

  //---------------------------
  ImGui::Separator();
}
void GUI_CloudPlayer::playCloud_byMouseWheel(){
  Cloud* cloud = database.cloud_selected;
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel - rolling stone
  if(io.MouseWheel && io.MouseDownDuration[1] == -1 && !io.WantCaptureMouse){
    if(cloud != nullptr){
      if(cloud->subset[0].ts.size() != 0){
        int subset_selected = *odoManager->get_frame_ID();

        if(io.MouseWheel > 0){
          subset_selected++;
        }else{
          subset_selected--;
        }

        odoManager->select_byFrameID(cloud, subset_selected);
      }
    }
  }

  //----------------------------
}
void GUI_CloudPlayer::parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    Cloud* cloud = database.cloud_selected;
    //---------------------------

    //Choice of the LiDAR model
    ImGui::SetNextItemWidth(75);
    static int lidar_model = 0;
    if (ImGui::Combo("##1", &lidar_model, "VLP-16\0HDL-32\0")){

    }

    if(cloud != nullptr){
      int* point_size = &cloud->point_size;
      ImGui::SliderInt("Point size", point_size, 1, 20);
    }

    bool* all_frame_visible = odoManager->get_all_frame_visible();
    if(ImGui::Checkbox("All frame visible", all_frame_visible)){
      if(cloud != nullptr){
        for(int i=0; i<cloud->nb_subset; i++){
          Subset* subset = &cloud->subset[i];
          subset->visibility = all_frame_visible;
        }
      }
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
