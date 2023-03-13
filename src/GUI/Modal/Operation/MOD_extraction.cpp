#include "MOD_extraction.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Scene/Glyph/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Cloud/Extraction.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_extraction::MOD_extraction(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->extractionManager = node_ope->get_extractionManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->glyphManager = node_scene->get_glyphManager();

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
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Extraction functions
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from AABB manipulators");
  bool* highlightON = extractionManager->get_highlightON();
  if(ImGui::Checkbox("Hightligth", highlightON)){
    if(collection != nullptr){
      Cloud* cloud = (Cloud*)collection->selected_obj;
      Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
      extractionManager->fct_highlighting(cloud, list_obj_init);
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
    if(collection != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", collection);
    }
  }
  if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    if(collection != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", collection);
    }
  }
  if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
    if(collection != nullptr){
      extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
      extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
      //glyphManager->update_glyph_object("aabb", collection);
    }
  }
  ImGui::PopAllowKeyboardFocus();

  //Extract a new cloud from AABB manipulators
  if(ImGui::Button("Extract cloud", ImVec2(100,0))){
    if(collection != nullptr){
      //Reset color
      *highlightON = false;
      Cloud* cloud = (Cloud*)collection->selected_obj;
      Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
      extractionManager->fct_highlighting(cloud, list_obj_init);

      //Extract cloud
      extractionManager->fct_extractCloud(collection);
    }
  }
  ImGui::SameLine();
  static bool sliceON = false;
  if(ImGui::Checkbox("Slice", &sliceON)){
    extractionManager->set_sliceON(sliceON);
  }

  //Cutting functions
  if(ImGui::Button("Cut", ImVec2(100,0))){
    if(collection != nullptr){
      //Reset color
      *highlightON = false;
      Cloud* cloud = (Cloud*)collection->selected_obj;
      Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
      extractionManager->fct_highlighting(cloud, list_obj_init);

      //Cut collection
      extractionManager->fct_cutCloud(cloud);
    }
  }
  ImGui::SameLine();
  if(ImGui::Button("Cut all cloud", ImVec2(100,0))){
    if(collection != nullptr){
      //Reset color
      *highlightON = false;
      Cloud* cloud = (Cloud*)collection->selected_obj;
      Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
      extractionManager->fct_highlighting(cloud, list_obj_init);

      //Cut clouds
      extractionManager->fct_cutCloud_all();
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_extraction::extract_with_mouse(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Extract points selected with the mouse frame
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from mouse frame");
  if(ImGui::Button("Extract selected frame", ImVec2(150,0))){
    if(collection != nullptr){
      extractionManager->fct_extractSelected(collection);
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_extraction::merge_cloud(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Merge and extract two clouds
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Merge and extract two clouds");
  if(ImGui::Button("Merge clouds", ImVec2(150,0))){
    list<Collection*>* list_collection = sceneManager->get_list_col_object();
    if(list_collection->size() >= 2){
      //Collection* cloud_2 = sceneManager->get_cloud_next();
      //extractionManager->fct_merging_newCloud(collection, cloud_2);
    }
  }

  //---------------------------
}
