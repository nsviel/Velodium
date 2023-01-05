#include "MOD_extraction.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Cloud/Extraction.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_extraction::MOD_extraction(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->extractionManager = node_ope->get_extractionManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();

  this->item_width = 150;

  //---------------------------
}
MOD_extraction::~MOD_extraction(){}

//Main function
void MOD_extraction::design_extractCloud(){
  if(modal_tab.show_extractCloud){
    ImGui::Begin("Extract cloud", &modal_tab.show_extractCloud,ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->extract_with_aabb();
    this->extract_with_mouse();
    this->merge_cloud();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      modal_tab.show_extractCloud = false;
    }
    ImGui::End();
  }
}

//Specific function
void MOD_extraction::extract_with_aabb(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Extraction functions
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from AABB manipulators");
  bool* highlightON = extractionManager->get_highlightON();
  if(ImGui::Checkbox("Hightligth", highlightON)){
    if(cloud != nullptr){
      Subset* subset = cloud->subset_selected;
      Subset* subset_init = sceneManager->get_subset_selected_init();
      extractionManager->fct_highlighting(subset, subset_init);
    }
  }
  ImGui::SameLine();

  //Reset manipulators
  static float xmin = 0, xmax = 100;
  static float ymin = 0, ymax = 100;
  static float zmin = 0, zmax = 100;
  if(ImGui::Button("Reset X Y Z", ImVec2(100,0))){
    xmin = 0;
    xmax = 100;
    ymin = 0;
    ymax = 100;
    zmin = 0;
    zmax = 100;
  }

  //AABB manipulators
  ImGui::PushAllowKeyboardFocus(false);
  if(ImGui::DragFloatRange2("X", &xmin, &xmax, 0.25f, 0.01f, 100.0f, "%.1f %%", "%.1f %%")){
    if(cloud != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", cloud);
    }
  }
  if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    if(cloud != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", cloud);
    }
  }
  if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    if(cloud != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", cloud);
    }
  }
  ImGui::PopAllowKeyboardFocus();

  //Extract a new cloud from AABB manipulators
  if(ImGui::Button("Extract cloud", ImVec2(100,0))){
    if(cloud != nullptr){
      //Reset color
      *highlightON = false;
      Subset* subset = cloud->subset_selected;
      Subset* subset_init = sceneManager->get_subset_selected_init();
      extractionManager->fct_highlighting(subset, subset_init);

      //Extract cloud
      extractionManager->fct_extractCloud(cloud);
    }
  }
  ImGui::SameLine();
  static bool sliceON = false;
  if(ImGui::Checkbox("Slice", &sliceON)){
    extractionManager->set_sliceON(sliceON);
  }

  //Cutting functions
  if(ImGui::Button("Cut", ImVec2(100,0))){
    if(cloud != nullptr){
      //Reset color
      *highlightON = false;
      Subset* subset = cloud->subset_selected;
      Subset* subset_init = sceneManager->get_subset_selected_init();
      extractionManager->fct_highlighting(subset, subset_init);

      //Cut cloud
      extractionManager->fct_cutCloud(subset);
    }
  }
  ImGui::SameLine();
  if(ImGui::Button("Cut all cloud", ImVec2(100,0))){
    if(cloud != nullptr){
      //Reset color
      *highlightON = false;
      Subset* subset = cloud->subset_selected;
      Subset* subset_init = sceneManager->get_subset_selected_init();
      extractionManager->fct_highlighting(subset, subset_init);

      //Cut clouds
      extractionManager->fct_cutCloud_all();
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_extraction::extract_with_mouse(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Extract points selected with the mouse frame
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from mouse frame");
  if(ImGui::Button("Extract selected frame", ImVec2(150,0))){
    if(cloud != nullptr){
      extractionManager->fct_extractSelected(cloud);
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_extraction::merge_cloud(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Merge and extract two clouds
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Merge and extract two clouds");
  if(ImGui::Button("Merge clouds", ImVec2(150,0))){
    list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
    if(list_cloud->size() >= 2){
      Cloud* cloud_2 = sceneManager->get_cloud_next();
      extractionManager->fct_merging_newCloud(cloud, cloud_2);
    }
  }

  //---------------------------
}
