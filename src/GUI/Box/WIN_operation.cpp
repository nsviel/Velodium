#include "WIN_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"
#include "../../Operation/Node_operation.h"
#include "../../Operation/Transformation/Filter.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Optimization/Fitting.h"
#include "../../Operation/Function/Extraction.h"
#include "../../Operation/Function/Selection.h"
#include "../../Specific/fct_transtypage.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_operation::WIN_operation(Node_operation* node_ope){
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
WIN_operation::~WIN_operation(){}

//Main function
void WIN_operation::window_filter(){
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
void WIN_operation::window_selection(){
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
void WIN_operation::window_transformation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  
  if(window_tab.show_transformation && cloud != nullptr){
    ImGui::Begin("Transformation", &window_tab.show_transformation, ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = cloud->subset_selected;
    //---------------------------

    //Z scanner
    /*ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Cloud elevation");

    //One or all cloud to operate
    static bool allClouds = false;
    ImGui::Checkbox("All clouds", &allClouds);

    static float Z_approx = 0.0f;
    static float Zpos = 0.0f;
    ImGui::PushItemWidth(75);
    if(ImGui::DragFloat("Z", &Zpos, 0.01f)){
      if(cloud != nullptr){
        transformManager->make_elevation(cloud, Zpos);
        sceneManager->update_cloud_location(cloud);
        Z_approx = transformManager->fct_soilDetermination(cloud);
      }
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Z : %.3f", Z_approx);

    //Scanner height from ground
    static float Z_scan = 0.0f;
    if(ImGui::DragFloat("Scanner height", &Z_scan, 0.05f)){
      if(cloud != nullptr){
        Z_approx = transformManager->fct_soilDetermination(cloud);
      }
    }
    ImGui::SameLine();
    static int soilnb_point = 10000;
    if(ImGui::DragInt("Ground pts", &soilnb_point, 100)){
      if(cloud != nullptr){
        transformManager->set_soilnb_point(soilnb_point);
        Z_approx = transformManager->fct_soilDetermination(cloud);
      }
    }

    if(ImGui::Button("Accept##0")){
      if(allClouds){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0;i<list_cloud->size();i++){
          Cloud* cloud = *next(list_cloud->begin(),i);
          transformManager->fct_adjustPosToScanner(cloud, Z_scan);
          sceneManager->update_cloud_location(cloud);
        }
      }
      else{
        if(cloud != nullptr){
          transformManager->fct_adjustPosToScanner(cloud, Z_scan);
          sceneManager->update_cloud_location(cloud);
          Z_approx = transformManager->fct_soilDetermination(cloud);
        }
      }
    }
    ImGui::Separator();*/
    //---------------------------

    //Transformation matrix from initial
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Actual transformation from initial position");
    MatrixXf cloudTransformation = glm_to_eigen_mat4_matXf(subset->transformation);
    std::stringstream ss;
    ss << cloudTransformation;
    string bla = ss.str();
    static char cloudTransformation_c[1024 * 16];
    strcpy(cloudTransformation_c, bla.c_str());
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine;
    ImGui::InputTextMultiline("##source", cloudTransformation_c, IM_ARRAYSIZE(cloudTransformation_c), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

    //Real transformation matrix registration
    if(ImGui::CollapsingHeader("Real transformation matrix")){
      MatrixXf realTransformation = glm_to_eigen_mat4_matXf(subset->transformation);//transformation.RealTransformation);
      std::stringstream ss1;
      ss1 << realTransformation;
      string bla1 = ss1.str();
      static char realTransformation_c[1024 * 16];
      strcpy(realTransformation_c, bla1.c_str());
      ImGui::InputTextMultiline("##realTransfo", realTransformation_c, IM_ARRAYSIZE(realTransformation_c), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

      if(ImGui::Button("Apply real transformation from init", ImVec2(300,0))){
        sceneManager->reset_cloud(cloud);
        transformManager->make_Transformation(subset, vec3(0,0,0), subset->transformation);//transformation.RealTransformation);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::Separator();
    //---------------------------

    //Applicable transformation matrix
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Applicable transformation matrix");
    static char TransfoMatrix[1024 * 16] =
      "1.000 0.000 0.000 0.000\n"
      "0.000 1.000 0.000 0.000\n"
      "0.000 0.000 1.000 0.000\n"
      "0.000 0.000 0.000 1.000\n";

    if(ImGui::Button("Reset##2", ImVec2(100,0))){
      strcpy(TransfoMatrix,
          "1.000 0.000 0.000 0.000\n"
          "0.000 1.000 0.000 0.000\n"
          "0.000 0.000 1.000 0.000\n"
          "0.000 0.000 0.000 1.000\n");
    }
    ImGui::SameLine();
    if(ImGui::Button("Transpose", ImVec2(100,0))){
      //Convert char* to string
      string str(TransfoMatrix);

      //Convert string to MatrixXf
      istringstream iss(str);
      float m0, m1, m2, m3;
      float m4, m5, m6, m7;
      float m8, m9, m10, m11;
      float m12, m13, m14, m15;
      iss >> m0 >> m1 >> m2 >> m3;
      iss >> m4 >> m5 >> m6 >> m7;
      iss >> m8 >> m9 >> m10 >> m11;
      iss >> m12 >> m13 >> m14 >> m15;

      MatrixXf mat(4,4);
      mat(0,0) = m0; mat(0,1) = m1; mat(0,2) = m2; mat(0,3) = m3;
      mat(1,0) = m4; mat(1,1) = m5; mat(1,2) = m6; mat(1,3) = m7;
      mat(2,0) = m8; mat(2,1) = m9; mat(2,2) = m10; mat(2,3) = m11;
      mat(3,0) = m12; mat(3,1) = m13; mat(3,2) = m14; mat(3,3) = m15;
      MatrixXf mat2 = mat.transpose();

      //Copy result on the gui
      std::stringstream ss;
      ss << mat2;
      string bla = ss.str();
      strcpy(TransfoMatrix, bla.c_str());
    }

    ImGui::InputTextMultiline("##source2", TransfoMatrix, IM_ARRAYSIZE(TransfoMatrix), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

    if(ImGui::Button("Apply from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        Cloud* cloud = sceneManager->get_selected_cloud();
        Subset* subset = cloud->subset_selected;

        sceneManager->reset_cloud(cloud);
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Apply from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        //------------------
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }
    if(ImGui::Button("Reverse from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        Cloud* cloud = sceneManager->get_selected_cloud();
        Subset* subset = cloud->subset_selected;

        sceneManager->reset_cloud(cloud);
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reverse from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::Separator();

    //---------------------------
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Cloud translation");
    ImGui::PushItemWidth(150);
    static float trans[3] = {0.0f, 0.0f, 0.0f};
    ImGui::DragFloat3("XYZ", trans, 0.01f, -10.0f, 10.0f);
    ImGui::SameLine();
    if(ImGui::Button("Apply##1")){
      if(cloud != nullptr){
        vec3 translation = vec3(trans[0], trans[1], trans[2]);
        transformManager->make_translation(cloud, translation);
        sceneManager->update_cloud_location(cloud);
        trans[0] = 0;
        trans[1] = 0;
        trans[2] = 0;
      }
    }
    ImGui::Separator();

    //---------------------------
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Cloud rotation");
    if(ImGui::Button("X ->")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(90, 0, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("X <-")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(-90, 0, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Y ->")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(0, 90, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Y <-")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(0, -90, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Z ->")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(0, 0, 90));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Z <-")){
      if(cloud != nullptr){
        vec3 radian = transformManager->fct_degreeToRadian(vec3(0, 0, -90));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::Separator();

    //---------------------------
    if(ImGui::Button("Close")){
      window_tab.show_transformation = false;
    }
    ImGui::End();
  }
}
void WIN_operation::window_fitting(){
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
void WIN_operation::window_extractCloud(){
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
void WIN_operation::window_cutCloud(){
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
