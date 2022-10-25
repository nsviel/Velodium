#include "WIN_attribut.h"

#include "IconsFontAwesome5.h"

#include "../Node_gui.h"
#include "../Control/GUI_Color.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"
#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Color/Heatmap.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Node_operation.h"
#include "../../Specific/fct_maths.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_attribut::WIN_attribut(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_engine* node_engine = node_gui->get_node_engine();

  this->gui_color = node_gui->get_gui_color();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();
  this->colorManager = node_ope->get_colorManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->transformManager = new Transforms();

  this->item_width = 150;

  //---------------------------
}
WIN_attribut::~WIN_attribut(){}

//Main function
void WIN_attribut::window_normal(){
  Cloud* cloud = sceneManager->get_selected_cloud();

  if(window_tab.show_normal && cloud != nullptr){
    ImGui::Begin("Attributs", &window_tab.show_normal,ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = cloud->subset_selected;
    Subset* subset_init = sceneManager->get_subset_init(cloud, cloud->ID_selected);
    //---------------------------

    if(ImGui::Button("Compute attributs for all clouds", ImVec2(200,0))){
      attribManager->compute_attribut_all();
    }

    //Standard normal computation
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Normals");

    static int normalMethod = 0;
    ImGui::PushItemWidth(207.5);
    ImGui::Combo("##11", &normalMethod, "PCL\0Hough\0Sphere fitting\0Plane fitting\0X axis\0Y axis\0Z axis\0");

    static float radius_normal = 0.03f;
    if(normalMethod == 1){
      ImGui::SetNextItemWidth(100);
      if(ImGui::InputFloat("Hough radius", &radius_normal, 0.01f, 1.0f, "%.3f")){
        if(radius_normal < 0.01) radius_normal = 0.01;
        //attribManager->set_normalRadiusSeach(radius_normal);
      }
    }
    if(normalMethod == 2){
      static float radius = attribManager->get_sphereRadius();
      ImGui::SetNextItemWidth(100);
      if(ImGui::DragFloat("Sphere radius", &radius, 0.0001, 0, 1, "%.4f")){
        attribManager->set_sphereRadius(radius);
      }
    }

    if(ImGui::Button("Compute", ImVec2(200,0))){
      if(cloud != nullptr){
        //---------------------------
        Subset* subset = cloud->subset_selected;
        Subset* subset_init = sceneManager->get_subset_init(cloud, cloud->ID_selected);

        if(normalMethod == 0){
          attribManager->compute_normals(subset);
        }

        if(normalMethod == 1){
          attribManager->compute_normals_Hough(subset);
        }

        if(normalMethod == 2){
          attribManager->compute_normals_sphere(subset);
        }

        if(normalMethod == 3){
          attribManager->compute_normals_planFitting(subset);
        }

        if(normalMethod == 4){
          float angle = transformManager->make_orientAxis_X(cloud);
          attribManager->compute_normals_planXaxis(subset);
          vec3 rotation = vec3(0, 0, -angle);
          transformManager->make_rotation(cloud, vec3(0,0,0), rotation);
          subset_init->N = subset->N;
          sceneManager->update_cloud_location(cloud);
        }

        if(normalMethod == 5){
          attribManager->compute_normals_planYaxis(subset);
          subset_init->N = subset->N;
          sceneManager->update_subset_location(subset);
        }

        if(normalMethod == 6){
          attribManager->compute_normals_planZaxis(subset);
          subset_init->N = subset->N;
          sceneManager->update_cloud_location(cloud);
        }

        //glyphManager->update_glyph_object("normal", cloud);

        //---------------------------
      }
    }
    if(ImGui::Button("Compute all clouds", ImVec2(200,0))){
      if(cloud != nullptr){
        //---------------------------


        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0;i<list_cloud->size();i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          if(normalMethod == 0){
            attribManager->compute_normals(subset);
          }

          if(normalMethod == 1){
            attribManager->compute_normals_Hough(subset);
          }

          if(normalMethod == 2){
            attribManager->compute_normals_sphere(subset);
          }

          if(normalMethod == 3){
            attribManager->compute_normals_planFitting(subset);
          }

          if(normalMethod == 4){
            float angle = transformManager->make_orientAxis_X(cloud);
            attribManager->compute_normals_planXaxis(subset);
            vec3 rotation = vec3(0, 0, -angle);
            transformManager->make_rotation(cloud, vec3(0,0,0), rotation);
            subset_init->N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          if(normalMethod == 5){
            attribManager->compute_normals_planYaxis(subset);
            subset_init->N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          if(normalMethod == 6){
            attribManager->compute_normals_planZaxis(subset);
            subset_init->N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          //glyphManager->update_glyph_object("normal", cloud);
        }

        //---------------------------
      }
    }

    if(ImGui::Button("Reoriente to origin", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_reorientToOrigin(subset);
        //glyphManager->update_glyph_object("normal", cloud);
      }
    }
    if(ImGui::Button("Invert", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_invert();
        //glyphManager->update_glyph_object("normal", cloud);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_normal = false;
    }
    ImGui::End();
  }
}
void WIN_attribut::window_intensity(){
  Cloud* cloud = sceneManager->get_selected_cloud();

  if(window_tab.show_intensity && cloud != nullptr){
    ImGui::Begin("Intensity", &window_tab.show_intensity, ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = cloud->subset_selected;
    Subset* subset_init = sceneManager->get_subset_selected_init();
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity functions");

    //Display intensity or color channel
    static bool colorON = false;
    if(ImGui::Button("Intensity / Color all", ImVec2(200,0))){
      colorON = !colorON;
      if(colorON){
        colorManager->set_color_RGB(cloud);
      }else{
        colorManager->set_color_I(cloud);
      }
    }

    //Invert the intensity values
    if(ImGui::Button("Inversion Is", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_intensityInversion();
        sceneManager->update_subset_IntensityToColor(subset);
      }
    }

    //fct_normalize the intensity values
    if(ImGui::Button("fct_normalize Intensity to [0,1]", ImVec2(200,0))){
      vector<float>& Is = subset->I;
      Is = fct_normalize(Is);
      sceneManager->update_subset_IntensityToColor(subset);
    }

    //Intensity display slider
    ImGui::Text("Selection intensity");
    static float min = 0, max = 1;
    if(ImGui::DragFloatRange2("##123321", &min, &max, 0.001f, 0.00f, 1.0f, "%.3f", "%.3f")){
      subset->I = subset_init->I;
      attribManager->fct_IsRange(vec2(min, max));
    }

    //Intensity shifting
    ImGui::Text("Shift intensity");
    static float shift = 0.01;
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("##123322", &shift, 0.0f, 1.0f, "%.4f");
    ImGui::SameLine();
    ImGuiStyle& style = ImGui::GetStyle();
    float spacing = style.ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if(ImGui::ArrowButton("##left", ImGuiDir_Left)){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i] - shift;
        if(Is[i] < 0.0f) Is[i] = 0.0f;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine(0.0f, spacing);
    if(ImGui::ArrowButton("##right", ImGuiDir_Right)){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i] + shift;
        if(Is[i] > 1.0f) Is[i] = 1.0f;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::PopButtonRepeat();
    ImGui::Separator();

    //Reconvert intensity
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity scaling");
    if(ImGui::Button("Restore I initial", ImVec2(200,0))){
      subset->I = subset_init->I;
      sceneManager->update_subset_IntensityToColor(subset);
    }
    if(ImGui::Button("I:255->2048", ImVec2(100,0))){
      attribManager->fct_convert255to2048(subset);
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine();
    if(ImGui::Button("I:2048->255", ImVec2(100,0))){
      attribManager->fct_convert2048to255(subset);
      sceneManager->update_subset_IntensityToColor(subset);
    }
    if(ImGui::Button("I:1->2048", ImVec2(100,0))){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i]*4096-2048;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine();
    if(ImGui::Button("I:2048->1", ImVec2(100,0))){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = (Is[i]+2048)/4096;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_intensity = false;
    }
    ImGui::End();
  }
}
void WIN_attribut::window_color(){
  Cloud* cloud = sceneManager->get_selected_cloud();

  if(window_tab.show_color && cloud != nullptr){
    ImGui::Begin("Colorization", &window_tab.show_color, ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = cloud->subset_selected;
    //---------------------------

    gui_color->colorization_choice();
    gui_color->heatmap_application();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_color = false;
    }
    ImGui::End();
  }
}
