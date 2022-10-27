#include "MOD_operation.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Filter.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Operation/Optimization/Fitting.h"
#include "../../../Operation/Function/Extraction.h"
#include "../../../Operation/Function/Selection.h"
#include "../../../Specific/fct_transtypage.h"

#include "../Modal_tab.h"
extern struct Modal_tab window_tab;


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
  this->transformManager = new Transforms();

  this->item_width = 150;

  //---------------------------
}
MOD_operation::~MOD_operation(){}

//Main function
void MOD_operation::window_filter(){
  bool* open = &window_tab.show_filter;
  if(*open){
    ImGui::Begin("Filter manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    //---------------------------

    //Cylinder cleaning filter
    if (ImGui::Button("Cylinder cleaning", ImVec2(item_width,0))){
      if(cloud != nullptr){
        filterManager->filter_cloud_cylinder(cloud);
      }
    }
    float* r_min = filterManager->get_cyl_r_min();
    float* r_max = filterManager->get_cyl_r_max();
    float* z_min = filterManager->get_cyl_z_min();
    ImGui::SameLine(); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
    ImGui::Dummy(ImVec2(item_width, 0.0f)); ImGui::SameLine(); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
    ImGui::Dummy(ImVec2(item_width, 0.0f)); ImGui::SameLine(); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("z min", z_min, 0.1f, 1.0f, "%.2f");

    //Filter by angle
    static int maxAngle = 80;
    if(ImGui::Button("Filter by angle", ImVec2(item_width,0))){
      if(cloud != nullptr){

        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);
          filterManager->filter_maxAngle(cloud, maxAngle);
          sceneManager->update_cloud_location(cloud);
        }

      }
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(item_width);
    ImGui::DragInt("##1", &maxAngle, 1, 0, 90, "%d°");

    //Sphere filtering
    if(ImGui::Button("Clean sphere cloud", ImVec2(item_width,0))){
      if(cloud != nullptr){
        filterManager->filter_sphereCleaning();
      }
    }
    ImGui::SameLine();
    static float sphereDiameter = 0.139f;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::DragFloat("##6", &sphereDiameter, 0.0001, 0, 2, "%.5f")){
      filterManager->set_sphereDiameter(sphereDiameter);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}
void MOD_operation::window_selection(){
  if(window_tab.show_selection){
    ImGui::Begin("Selection part", &window_tab.show_selection,ImGuiWindowFlags_AlwaysAutoResize);
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
      window_tab.show_selection = false;
    }
    ImGui::End();
  }
}
void MOD_operation::window_fitting(){
  if(window_tab.show_fitting){
    ImGui::Begin("Fitting", &window_tab.show_fitting,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    int sizeButton = 150;
    //---------------------------

    //Sphere fitting
    if(ImGui::Button("Sphere fitting", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        fitManager->Sphere_cloudToCenter_all();
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
        transformManager->make_orientAxis_X(cloud);
        transformManager->make_alignAxis_X(cloud);
        sceneManager->update_cloud_location(cloud);
      }
    }

    //---------------------------
    if(ImGui::Button("Close")){
      window_tab.show_fitting = false;
    }
    ImGui::End();
  }
}
void MOD_operation::window_extractCloud(){
  if(window_tab.show_extractCloud){
    ImGui::Begin("Extract cloud", &window_tab.show_extractCloud,ImGuiWindowFlags_AlwaysAutoResize);
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
    ImGui::Separator();

    //Extract points selected with the mouse frame
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from mouse frame");
    if(ImGui::Button("Extract selected frame", ImVec2(150,0))){
      if(cloud != nullptr){
        extractionManager->fct_extractSelected(cloud);
      }
    }
    ImGui::Separator();

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
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_extractCloud = false;
    }
    ImGui::End();
  }
}
void MOD_operation::window_cutCloud(){
  if(window_tab.show_cutCloud){
    ImGui::Begin("Cut cloud", &window_tab.show_cutCloud,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    //---------------------------

    bool* highlightON = extractionManager->get_highlightON();
    if(ImGui::Checkbox("Hightligth", highlightON) || ImGui::IsKeyPressed(258)){
      if(cloud != nullptr){
        Subset* subset = cloud->subset_selected;
        Subset* subset_init = sceneManager->get_subset_selected_init();
        extractionManager->fct_highlighting(subset, subset_init);
      }
    }
    ImGui::SameLine();

    //Reset manipulator
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

    //Cuttinf functions
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
    if(ImGui::Button("Close")){
      window_tab.show_cutCloud = false;
    }
    ImGui::End();
  }
}
