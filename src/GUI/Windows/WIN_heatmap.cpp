#include "WIN_heatmap.h"

#include "../../Engine/Scene.h"
#include "../../Operation/Functions/Heatmap.h"

#include "IconsFontAwesome5.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_heatmap::WIN_heatmap(Heatmap* heat){
  //---------------------------

  this->heatmapManager= heat;
  this->sceneManager = new Scene();

  //---------------------------
}
WIN_heatmap::~WIN_heatmap(){}

//Main function
void WIN_heatmap::window_heatmap(){
  if(window_tab.show_heatmap){
    ImGui::Begin(ICON_FA_EYE " Heatmap", &window_tab.show_heatmap, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Apply heatMap on one cloud
    if(ImGui::Button("Apply", ImVec2(75,0))){
      if(cloud != nullptr){
        heatmapManager->set_Heatmap(cloud);
      }
    }
    ImGui::SameLine();

    //Heatmap all clouds
    static bool heatAll = false;
    if(ImGui::Button("Apply all", ImVec2(75,0))){
      if(cloud != nullptr){
        heatAll = !heatAll;
        heatmapManager->set_Heatmap_all(heatAll);
      }
    }

    //Select heatmap channel
    static int style_idx = 0;

    int* HMmode = heatmapManager->get_HeatmapField();
    ImGui::SetNextItemWidth(75);
    ImGui::Combo("##1", HMmode, "height\0Is\0dist\0cos(It)\0It\0");
    ImGui::SameLine();

    //Normalize palette
    bool* normalizeON = heatmapManager->get_param_Normalized();
    ImGui::Checkbox("Normalized", normalizeON);

    //Height range configuration
    if(*HMmode == 0){
      vec2* range = heatmapManager->get_height_range();
      if(ImGui::DragFloatRange2("Height range", &range->x, &range->y, 0.01f, -20.0f, 50.0f, "%.2f", "%.2f")){
        heatmapManager->compute_subset_heatmap_ON(subset);
        sceneManager->update_subset_color(subset);
      }
    }

    //Intensity range configuration
    if(*HMmode == 1){
      //Set heatmap range
      vec2 heatmap_range = *heatmapManager->get_heatmap_range();
      int min = (int) (heatmap_range.x * 255);
      int max = (int) (heatmap_range.y * 255);

      if(ImGui::DragIntRange2("Intensity", &min, &max, 1, 0, 255, "%d", "%d")){
        vec2* range = heatmapManager->get_heatmap_range();
        *range = vec2((float)min / 255, (float)max / 255);
      }
    }

    //Display color palette
    if(ImGui::Button("Palette", ImVec2(75,0))){
      if(cloud != nullptr && cloud->heatmap){
        heatmapManager->plot_colorPalette(subset);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_heatmap = false;
    }
    ImGui::End();
  }
}
