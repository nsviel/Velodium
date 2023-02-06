#include "MOD_boxing.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Engine/Scene/Glyph/Object.h"
#include "../../../Engine/Scene/Glyph/Cloud/Box.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Cloud/Extraction.h"
#include "../../../Operation/Cloud/Boxing.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_boxing::MOD_boxing(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->objectManager = node_engine->get_objectManager();
  this->extractionManager = node_ope->get_extractionManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->boxingManager = node_ope->get_boxingManager();

  this->item_width = 150;

  //---------------------------
}
MOD_boxing::~MOD_boxing(){}

//Main function
void MOD_boxing::design_boxing(){
  if(modal_tab.show_boxing){
    ImGui::Begin("Boxing", &modal_tab.show_boxing, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->box_shape();
    this->control_box();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      modal_tab.show_boxing = false;
      boxingManager->stop_boxing();
    }
    ImGui::End();
  }
}

//Specific function
void MOD_boxing::box_shape(){
  Box* box = objectManager->get_object_box();
  Glyph* glyph = box->get_glyph();
  //---------------------------

  static int box_shape = 0;
  if(ImGui::RadioButton("Wire", &box_shape, 0)){
    glyph->draw_type = "line";
    box->build_box_location();
    glyphManager->update_glyph_location(box->get_glyph());
  }
  ImGui::SameLine();
  if(ImGui::RadioButton("Plain", &box_shape, 1)){
    glyph->draw_type = "triangle";
    box->build_box_location();
    glyphManager->update_glyph_location(box->get_glyph());
  }

  //---------------------------
}
void MOD_boxing::control_box(){
  //---------------------------

  //Reset manipulators
  static float xmin = 0, xmax = 100;
  static float ymin = 0, ymax = 100;
  static float zmin = 0, zmax = 100;
  if(ImGui::Button("Reset", ImVec2(100,0))){
    xmin = 0;
    ymin = 0;
    zmin = 0;

    xmax = 100;
    ymax = 100;
    zmax = 100;
  }

  //AABB manipulators
  ImGui::PushAllowKeyboardFocus(false);
  if(ImGui::DragFloatRange2("X", &xmin, &xmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    this->change_box(xmin, ymin, zmin, xmax, ymax, zmax);
  }
  if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    this->change_box(xmin, ymin, zmin, xmax, ymax, zmax);
  }
  if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    this->change_box(xmin, ymin, zmin, xmax, ymax, zmax);
  }
  ImGui::PopAllowKeyboardFocus();

  //---------------------------
}
void MOD_boxing::change_box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(cloud != nullptr){
    vec3 min_perc = vec3(xmin, ymin, zmin);
    vec3 max_perc = vec3(xmax, ymax, zmax);
    boxingManager->compute_box_MinMax(cloud, min_perc, max_perc);
    boxingManager->compute_visibility(cloud);
  }

  //---------------------------
}
