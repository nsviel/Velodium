#include "MOD_operation.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Filter.h"
#include "../../../Operation/Transformation/Pose.h"
#include "../../../Operation/Optimization/Fitting.h"
#include "../../../Operation/Cloud/Extraction.h"
#include "../../../Operation/Cloud/Selection.h"
#include "../../../Specific/fct_transtypage.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_operation::MOD_operation(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->filterManager = node_ope->get_filterManager();
  this->selectionManager = node_ope->get_selectionManager();
  this->fitManager = node_ope->get_fittingManager();
  this->extractionManager = node_ope->get_extractionManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->poseManager = new Pose();

  this->item_width = 150;

  //---------------------------
}
MOD_operation::~MOD_operation(){}

//Main function
void MOD_operation::window_selection(){
  if(modal_tab.show_selection){
    ImGui::Begin("Selection part", &modal_tab.show_selection,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    //---------------------------

    ImGui::Text("Point");
    static bool selectionPtON = false;
    if(ImGui::Checkbox("Selection mode", &selectionPtON)){
      if(cloud != nullptr && selectionPtON){
        selectionManager->set_markMode("sphere");
        cloud->point_size = 10;
        //cloud_movement = false;
      }

      if(!selectionPtON){
        selectionManager->set_markMode("cube");
        cloud->point_size = 1;
        //cloud_movement = true;
      }
    }
    static float sensibility = 0.005f;
    if(ImGui::DragFloat("Sensibility", &sensibility, 0.0001, 0, 1, "%.4f")){
      selectionManager->set_selectionSensibility(sensibility);
    }
    if(ImGui::Button("Supress all points", ImVec2(200,0))){
      selectionManager->mark_supressSelectedPoints_all();

    }
    ImGui::Separator();

    ImGui::Text("Cloud part");
    static float xmin = 0, xmax = 100;
    static float ymin = 0, ymax = 100;
    static float zmin = 0, zmax = 100;
    if(ImGui::Button("Reset", ImVec2(100,0)) || ImGui::IsKeyPressed(258)){
      xmin = 0; xmax = 100;
      ymin = 0; ymax = 100;
      zmin = 0; zmax = 100;
    }
    ImGui::SameLine();
    bool* highlightON = extractionManager->get_highlightON();
    if(ImGui::Checkbox("Hightligth", highlightON) || ImGui::IsKeyPressed(258)){
      if(cloud != nullptr){
        Subset* subset = cloud->subset_selected;
        Subset* subset_init = sceneManager->get_subset_selected_init();
        extractionManager->fct_highlighting(subset, subset_init);
      }
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

    if(ImGui::Button("Extract part", ImVec2(100,0))){
      if(cloud != nullptr){
        vec3 min_pourc = vec3(xmin, ymin, zmin);
        vec3 max_pourc = vec3(xmax, ymax, zmax);
        extractionManager->fct_selectPart(subset, min_pourc, max_pourc);
      }
    }

    //Table of selected parts
    list<subpart*>* list = extractionManager->get_listParts();
    ImGui::Columns(5, "part");
    ImGui::Separator();
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Cloud"); ImGui::NextColumn();
    ImGui::Text("Min"); ImGui::NextColumn();
    ImGui::Text("Max"); ImGui::NextColumn();
    ImGui::Separator();
    static int selected = -1;

    ImGui::SetColumnWidth(0,50);
    ImGui::SetColumnWidth(1,125);

    for (int i=0; i<list->size(); i++){
      subpart* part = *next(list->begin(),i);

      //ID
      char label[32];
      sprintf(label, "%04d", i);
      if(ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_None)){
        selected = i;
      }
      bool hovered = ImGui::IsItemHovered();
      ImGui::NextColumn();

      //Name
      static char str[256];
      strcpy(str, part->name.c_str());
      string truc = "##" + to_string(i);
      const char* ID_label = truc.c_str();
      if(ImGui::InputText(ID_label, str, IM_ARRAYSIZE(str))){
        part->name = str;
      }

      //Removal cross
      ImGui::PushID(i);
      ImGui::SameLine();
      if(ImGui::SmallButton("X")){
        extractionManager->supress_selectedpart(part);
      }
      ImGui::PopID();
      ImGui::NextColumn();

      //Cloud
      ImGui::Text("%s", part->namePC.c_str());
      ImGui::NextColumn();

      //Min
      vec3 minloc = part->minloc;
      ImGui::Text("%.1f %.1f %.1f", minloc.x, minloc.y, minloc.z);
      ImGui::NextColumn();

      //Max
      vec3 maxloc = part->maxloc;
      ImGui::Text("%.1f %.1f %.1f", maxloc.x, maxloc.y, maxloc.z);
      ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();

    //---------------------------
    if(ImGui::Button("Close")){
      modal_tab.show_selection = false;
    }
    ImGui::End();
  }
}
void MOD_operation::window_fitting(){
  if(modal_tab.show_fitting){
    ImGui::Begin("Fitting", &modal_tab.show_fitting,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    int sizeButton = 150;
    //---------------------------

    //Sphere fitting
    if(ImGui::Button("Sphere fitting", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        fitManager->Sphere_cloudToCenter_all(list_cloud);
      }
    }

    //Plane fitting
    if(ImGui::Button("Plane fitting", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        //fitManager->Plane_cloud(subset);
      }
    }

    //Axis alignement
    if(ImGui::Button("X alignement", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        poseManager->make_orientAxis_X(cloud);
        poseManager->make_alignAxis_X(cloud);
        sceneManager->update_cloud_location(cloud);
      }
    }

    //---------------------------
    if(ImGui::Button("Close")){
      modal_tab.show_fitting = false;
    }
    ImGui::End();
  }
}
