#include "GUI_Color.h"

#include "../Color.h"
#include "../Heatmap.h"

#include "../../Operation_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../GUI/GUI_node.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Color::GUI_Color(GUI_node* node_gui){
  //---------------------------

  Engine_node* node_engine = node_gui->get_node_engine();
  Operation_node* node_ope = node_gui->get_node_ope();

  this->colorManager = node_ope->get_colorManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->sceneManager = node_engine->get_sceneManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Color::~GUI_Color(){}


void GUI_Color::colorization_choice(){
  //---------------------------

  int* color_mode = colorManager->get_color_mode();
  ImGui::RadioButton("Unicolor", color_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Intensity", color_mode, 1);
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
}
void GUI_Color::option_intensity(){
  //---------------------------

  //Set heatmap range
  vec2 intensity_range = *heatmapManager->get_range_intensity();
  int min = (int) (intensity_range.x * 255);
  int max = (int) (intensity_range.y * 255);

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
  if(ImGui::DragIntRange2("Intensity", &min, &max, 1, 0, 255, "%d", "%d")){
    vec2* range = heatmapManager->get_range_normalization();
    *range = vec2((float) (min / 255), (float) (max / 255));
  }

  //---------------------------
}
void GUI_Color::option_heatmap(){
  //---------------------------

  int* heatmap_mode = heatmapManager->get_heatmap_mode();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
  ImGui::RadioButton("Height", heatmap_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Intensity", heatmap_mode, 1);
  ImGui::SameLine();
  ImGui::RadioButton("Distance", heatmap_mode, 2);

  //Heatmap - Height
  if(*heatmap_mode == 0){
    vec2* height_range = heatmapManager->get_range_height();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("Z min", &height_range->x, 0.1f, 1.0f, "%.2f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("Z max", &height_range->y, 0.1f, 1.0f, "%.2f");
  }

  //Heatmap - Intensity
  if(*heatmap_mode == 1){
    //Set heatmap range
    vec2 intensity_range = *heatmapManager->get_range_intensity();
    int min = (int) (intensity_range.x * 255);
    int max = (int) (intensity_range.y * 255);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::DragIntRange2("Intensity", &min, &max, 1, 0, 255, "%d", "%d")){
      vec2* range = heatmapManager->get_range_intensity();
      *range = vec2((float)min / 255, (float)max / 255);
    }
  }

  //---------------------------
}