#include "GUI_Color.h"

#include "../Node_gui.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Color/Colormap.h"
#include "../../Operation/Color/Heatmap.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Color::GUI_Color(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_operation* node_ope = node_gui->get_node_ope();

  this->colorManager = node_ope->get_colorManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->colormapManager = heatmapManager->get_colormapManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Color::~GUI_Color(){}

//Main function
void GUI_Color::colorization_choice(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Colorization");
  //---------------------------

  int* color_mode = colorManager->get_color_mode();
  ImGui::RadioButton("Unicolor", color_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Intensity##0", color_mode, 1);
  ImGui::SameLine();
  ImGui::RadioButton("Heatmap", color_mode, 2);

  //Option: heatmap with relative height
  if(*color_mode == 1){
    this->option_intensity();
  }

  //Option: Intensity range
  if(*color_mode == 2){
    this->option_heatmap();
  }

  //---------------------------
  ImGui::Separator();
}

//Option functions
void GUI_Color::option_intensity(){
  //---------------------------

  //Set heatmap range
  vec2 intensity_range = *colorManager->get_range_intensity();
  int min = (int) (intensity_range.x * 255);
  int max = (int) (intensity_range.y * 255);

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
  if(ImGui::DragIntRange2("Intensity##0001", &min, &max, 1, 0, 255, "%d", "%d")){
    vec2* range = colorManager->get_range_intensity();
    *range = vec2((float)min / 255, (float)max / 255);
  }

  //---------------------------
}
void GUI_Color::option_heatmap(){
  //---------------------------

  this->heatmap_mode();
  this->heatmap_select_colormap();
  this->heatmap_mode_height();
  this->heatmap_mode_intensity();

  //---------------------------
}
void GUI_Color::heatmap_application(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Heatmap");
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Apply heatMap on one cloud
  if(ImGui::Button("Apply", ImVec2(75,0))){
    if(cloud != nullptr){
      heatmapManager->make_cloud_heatmap(cloud);
    }
  }
  ImGui::SameLine();

  //Heatmap all clouds
  static bool heatAll = false;
  if(ImGui::Button("Apply all", ImVec2(75,0))){
    if(cloud != nullptr){
      heatAll = !heatAll;
      heatmapManager->make_heatmap_all(heatAll);
    }
  }

  //---------------------------
}
void GUI_Color::heatmap_select_colormap(){
  //---------------------------

  //Select colormap
  static int colormap = 0;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width);
  if(ImGui::Combo("##144", &colormap, "Viridis\0Magma\0Rainbow\0")){
    switch(colormap){
      case 0:{
        colormapManager->choose("viridis");
        break;
      }
      case 1:{
        colormapManager->choose("magma");
        break;
      }
      case 2:{
        colormapManager->choose("rainbow");
        break;
      }
    }
  }
  ImGui::SameLine();

  //Colormap normalization
  bool* normalizeON = heatmapManager->get_is_normalization();
  ImGui::Checkbox("fct_normalized", normalizeON);

  //---------------------------
}

void GUI_Color::heatmap_mode(){
  //---------------------------

  //Heatmap mode
  int* heatmap_mode = heatmapManager->get_heatmap_mode();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
  ImGui::RadioButton("Height", heatmap_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Intensity##2", heatmap_mode, 1);
  ImGui::SameLine();
  ImGui::RadioButton("Distance", heatmap_mode, 2);

  //---------------------------
}
void GUI_Color::heatmap_mode_height(){
  int* heatmap_mode = heatmapManager->get_heatmap_mode();
  //---------------------------

  if(*heatmap_mode == 0){
    vec2* height_range = heatmapManager->get_range_height();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Z min", &height_range->x, 0.1f, 1.0f, "%.2f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width);
    ImGui::InputFloat("Z max", &height_range->y, 0.1f, 1.0f, "%.2f");
  }

  //---------------------------
}
void GUI_Color::heatmap_mode_intensity(){
  int* heatmap_mode = heatmapManager->get_heatmap_mode();
  //---------------------------

  if(*heatmap_mode == 1){
    //Set heatmap range
    vec2* intensity_range = heatmapManager->get_range_intensity();
    int min = (int) (intensity_range->x * 255);
    int max = (int) (intensity_range->y * 255);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::DragIntRange2("Intensity##3", &min, &max, 1, 0, 255, "%d", "%d")){
      *intensity_range = vec2((float)min / 255, (float)max / 255);
    }
  }

  //---------------------------
}
