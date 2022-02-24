#include "WIN_attribut.h"

#include "IconsFontAwesome5.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"
#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Operation_node.h"
#include "../../Specific/fct_maths.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_attribut::WIN_attribut(Operation_node* node_ope){
  //---------------------------

  Engine_node* node_engine = node_ope->get_node_engine();

  this->heatmapManager = node_ope->get_heatmapManager();
  this->sceneManager = node_engine->get_SceneManager();
  this->attribManager = node_ope->get_attribManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->transformManager = new Transforms();

  this->item_width = 150;

  //---------------------------
}
WIN_attribut::~WIN_attribut(){}

//Main function
void WIN_attribut::window_normal(){
  Cloud* cloud = sceneManager->get_cloud_selected();

  if(window_tab.show_normal && cloud != nullptr){
    ImGui::Begin("Attributs", &window_tab.show_normal,ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = sceneManager->get_subset_selected();
    Subset* subset_init = sceneManager->get_subset_init(cloud, cloud->ID_selected);
    //---------------------------

    if(ImGui::Button("Compute attributs for all clouds", ImVec2(200,0))){
      attribManager->compute_cloudAttributs_all();
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
        Subset* subset = sceneManager->get_subset_selected();
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

        glyphManager->update_glyph_object("normal", cloud);

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

          glyphManager->update_glyph_object("normal", cloud);
        }

        //---------------------------
      }
    }

    if(ImGui::Button("Reoriente to origin", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_reorientToOrigin(subset);
        glyphManager->update_glyph_object("normal", cloud);
      }
    }
    if(ImGui::Button("Invert", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_invert();
        glyphManager->update_glyph_object("normal", cloud);
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
  Cloud* cloud = sceneManager->get_cloud_selected();

  if(window_tab.show_intensity && cloud != nullptr){
    ImGui::Begin("Intensity", &window_tab.show_intensity, ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = sceneManager->get_subset_selected();
    Subset* subset_init = sceneManager->get_subset_selected_init();
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity functions");

    //Display intensity or color channel
    static bool colorON = false;
    if(ImGui::Button("Intensity / Color all", ImVec2(200,0))){
      colorON = !colorON;
      if(colorON){
        attribManager->set_colorRGB_all();
      }else{
        attribManager->set_colorI_all();
      }
    }

    //Invert the intensity values
    if(ImGui::Button("Inversion Is", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_intensityInversion();
        sceneManager->update_subset_IntensityToColor(subset);
      }
    }

    //Normalize the intensity values
    if(ImGui::Button("Normalize Intensity to [0,1]", ImVec2(200,0))){
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
  Cloud* cloud = sceneManager->get_cloud_selected();

  if(window_tab.show_color && cloud != nullptr){
    ImGui::Begin("Color", &window_tab.show_color, ImGuiWindowFlags_AlwaysAutoResize);
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Color channel
    ImGui::Text("Color channel");
    static int e = 0;
    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Selected cloud"); ImGui::NextColumn();
    ImGui::Text("All cloud"); ImGui::NextColumn();
    if(ImGui::RadioButton("I    ##1", &e, 1)){
      if(subset->I.size() != 0){
        sceneManager->update_subset_IntensityToColor(subset);
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("I    ##2", &e, 2)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->I.size() != 0){
          sceneManager->update_subset_IntensityToColor(subset);
        }
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB  ##1", &e, 3)){
      if(subset->has_color){
        subset->RGB = subset->RGB;
        sceneManager->update_subset_color(subset);
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB  ##2", &e, 4)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->has_color){
          subset->RGB = subset->RGB;
          sceneManager->update_subset_color(subset);
        }
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB*I##1", &e, 5)){
      if(subset->I.size() != 0 && subset->has_color){
        attribManager->set_enhancedColor(subset);
      }else{
        cout<<"Selected cloud: I="<<subset->I.size()<<" | Color="<<subset->has_color<<endl;
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB*I##2", &e, 6)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->I.size() != 0 && subset->has_color){
          attribManager->set_enhancedColor(subset);
        }
      }
    } ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::Separator();

    //Color functions
    if(ImGui::Button("Supress color all clouds", ImVec2(150,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        for(int i=0;i<list_cloud->size();i++){
          Subset* subset = sceneManager->get_subset(cloud,i);
          if(subset->has_color){
            subset->RGB.clear();
          }
          subset->has_color = false;
        }

        sceneManager->update_cloud_dataFormat(cloud);
      }
    }
    if(ImGui::Button("Random color for all points", ImVec2(150,0))){
      attribManager->set_randomizeAllPointsColor(subset);
      sceneManager->update_subset_color(subset);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_color = false;
    }
    ImGui::End();
  }
}
void WIN_attribut::window_heatmap(){
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
