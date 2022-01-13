#include "GUI_Option.h"

#include "../Node/GUI_Control.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Glyphs.h"
#include "../../Engine/Configuration/Configuration.h"
#include "../../Operation/Operation.h"
#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"

#include "../../../extern/imgui/imgui.h"


//Constructor / Destructor
GUI_option::GUI_option(Engine* engine, GUI_control* control){
  this->gui_controlManager = control;
  this->engineManager = engine;
  //---------------------------

  this->heatmapManager = new Heatmap();
  this->glyphManager = new Glyphs();
  this->sceneManager = new Scene();
  this->attribManager = new Attribut;
  this->opeManager = new Operation();

  //---------------------------
}
GUI_option::~GUI_option(){}

//Main function
void GUI_option::design_Options(){
  //---------------------------

  this->option_font();
  this->option_glyphs();
  this->option_mode();
  this->option_heatmap();
  this->option_parameters();
  this->option_colors();

  //---------------------------
}

//Subfunctions
void GUI_option::option_glyphs(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  ImGuiStyle& style = ImGui::GetStyle();
  ImGui::Columns(2);
  //---------------------------

  //Display grid
  static bool gridON = true;
  if(ImGui::Checkbox("Grid", &gridON)){
    glyphManager->set_visibility("grid", gridON);
  }
  ImGui::NextColumn();

  //Subgrid
  static bool subGridON = false;
  if(ImGui::Checkbox("Subgrid", &subGridON)){
    glyphManager->set_visibility("subgrid", subGridON);
    glyphManager->set_visibility("planegrid", subGridON);
  }
  ImGui::NextColumn();

  //Display Bounding Box
  static bool aabbON = true;
  if(ImGui::Checkbox("AABB", &aabbON)){
    glyphManager->set_visibility("aabb", aabbON);
  }
  ImGui::NextColumn();

  //Display normals
  static bool normalsON = false;
  if(ImGui::Checkbox("Normal", &normalsON)){
    glyphManager->set_visibility_normal(normalsON);
  }
  ImGui::NextColumn();

  //Display ICP line correspondences
  static bool matchingON = false;
  if(ImGui::Checkbox("Match", &matchingON)){
    glyphManager->set_visibility("matching", matchingON);
  }
  ImGui::NextColumn();

  //Display Axis
  static bool axisON = true;
  if(ImGui::Checkbox("Axis", &axisON)){
    glyphManager->set_visibility("axis", axisON);
    glyphManager->set_visibility_axisCloud(axisON);
  }
  ImGui::NextColumn();

  ImGui::Columns(1);

  //Axis circle
  static float circleRadius = 1;
  ImGui::PushItemWidth(50);
  if(ImGui::DragFloat("##456", &circleRadius, 0.001, 0, 5, "%.3f")){
    //glyphManager->obj_axisCircle(circleRadius);
  }
  ImGui::SameLine();

  //Display Axis circle
  static bool axisCircleON = false;
  if(ImGui::Checkbox("Axis circle", &axisCircleON)){
    //glyphManager->obj_axisCircle(circleRadius);
    //glyphManager->set_visibility("axisCircle", axisCircleON);
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_option::option_heatmap(){
  if(ImGui::CollapsingHeader("Heatmap")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Heatmap
    if(ImGui::Button("Apply##238", ImVec2(75,0))){
      if(sceneManager->is_atLeastOnecloud()){
        heatmapManager->set_Heatmap(cloud);
      }
    }
    ImGui::SameLine();

    //Heatmap all
    static bool heatAll = false;
    if(ImGui::Button("Apply all", ImVec2(75,0))){
      if(sceneManager->is_atLeastOnecloud()){
        heatAll = !heatAll;
        heatmapManager->set_Heatmap_all(heatAll);
      }
    }

    int* HMmode = heatmapManager->get_HeatmapField();
    ImGui::SetNextItemWidth(75);
    ImGui::Combo("##1", HMmode, "height\0Is\0dist\0cos(It)\0It\0");
    ImGui::SameLine();

    //Normalize heatmap
    bool* normalizeON = heatmapManager->get_param_Normalized();
    ImGui::Checkbox("Normalized", normalizeON);

    //Display palette color
    if(ImGui::Button("Palette", ImVec2(75,0))){
      if(cloud != nullptr && cloud->heatmap){
        heatmapManager->plot_colorPalette(subset);
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_option::option_colors(){
  if(ImGui::CollapsingHeader("Colors")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    int colorEditSize = 150;
    //---------------------------

    //Background color
    ImGui::SetNextItemWidth(colorEditSize);
    ImGui::ColorEdit4("Background", (float*)backgColor);

    //Normals color
    ImGui::SetNextItemWidth(colorEditSize);
    vec4* color_normals = glyphManager->get_glyph_color("normal");
    if(ImGui::ColorEdit4("Normals", (float*)color_normals)){
      glyphManager->set_glyph_color("normal", *color_normals);
    }

    //Grid color
    ImGui::SetNextItemWidth(colorEditSize);
    vec4* color_grid = glyphManager->get_glyph_color("grid");
    if(ImGui::ColorEdit4("Grid", (float*)color_grid)){
      glyphManager->set_glyph_color("grid", *color_grid);
    }

    //Bounding box color
    ImGui::SetNextItemWidth(colorEditSize);
    vec4* color_aabb = glyphManager->get_glyph_color("aabb");
    if(ImGui::ColorEdit4("AABB", (float*)color_aabb)){
      glyphManager->set_glyph_color("aabb", *color_aabb);
    }

    //Uniform cloud color
    ImGui::SetNextItemWidth(colorEditSize);
    static vec4 color_PC;
    if(ImGui::ColorEdit4("Point cloud", (float*)&color_PC, ImGuiColorEditFlags_AlphaBar)){
      if(sceneManager->is_atLeastOnecloud()){
        Subset* subset = &cloud->subset[cloud->subset_selected];
        attribManager->set_pointCloudColor(subset, color_PC);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Reset", ImVec2(75,0))){
      Configuration configManager;
      float bkg_color = configManager.parse_json_float("window", "background_color");
      this->backgColor->x = bkg_color;
      this->backgColor->y = bkg_color;
      this->backgColor->z = bkg_color;
      glyphManager->reset_colors();
    }
  }
}
void GUI_option::option_parameters(){
  if(ImGui::CollapsingHeader("Parameters")){
    Cloud* cloud = sceneManager->get_cloud_selected();
    ImGuiStyle& style = ImGui::GetStyle();
    //---------------------------

    //cloud movement
    float* transCoef = gui_controlManager->get_transCoef();
    ImGui::DragFloat("Translation", transCoef, 0.001, 0, 100, "%.4f");
    float* rotatDegree = gui_controlManager->get_rotatDegree();
    ImGui::DragFloat("Rotation", rotatDegree, 0.5, 0, 90, "%.4f");

    //Point cloud scaling
    static float scale = 1.0f;
    ImGui::SetNextItemWidth(100);
    if(ImGui::DragFloat("Scale", &scale, 0.01, 0.1, 10, "%.2f X")){
      if(sceneManager->is_atLeastOnecloud()){
        Transforms transformManager;
        transformManager.make_scaling(cloud, scale);
        sceneManager->update_cloud_location(cloud);
      }
    }

    //Point shape
    static int point_shape = 0;
    if(ImGui::Combo("Point shape", &point_shape, "Quad\0Circle\0")){
      if(point_shape == 0){
        glDisable(GL_POINT_SMOOTH);
      }
      else if(point_shape == 1){
        glEnable(GL_POINT_SMOOTH);
      }
    }

    //Point size
    ImGui::Columns(2);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Points size ");
    ImGui::NextColumn();
    static int cpt_pts = 1;
    float spacing = style.ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)){
      cpt_pts--;
      if(cpt_pts<=1) cpt_pts=1;
      cloud->point_size = cpt_pts;
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)){
      cpt_pts++;
      cloud->point_size = cpt_pts;
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", cpt_pts);
    ImGui::NextColumn();

    //Normals size
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Normal size ");
    ImGui::NextColumn();
    static int cpt_nor = 1;
    float spacing_n = style.ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if(ImGui::ArrowButton("##left_n", ImGuiDir_Left)){
      cpt_nor--;
      if(cpt_nor <= 1){
        cpt_nor = 1;
      }
      glyphManager->set_size_normal(cpt_nor);
    }
    ImGui::SameLine(0.0f, spacing_n);
    if(ImGui::ArrowButton("##right_n", ImGuiDir_Right)){
      cpt_nor++;
      glyphManager->set_size_normal(cpt_nor);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", cpt_nor);
    ImGui::Columns(1);

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_option::option_mode(){
  if(ImGui::CollapsingHeader("Mode")){
    //---------------------------

    //Light / Dark mode
    static bool darkMode = false;
    if(ImGui::Checkbox("Dark mode", &darkMode)){
      if(darkMode == true){
        glyphManager->set_glyph_color("aabb",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glyphManager->set_glyph_color("selection",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        *backgColor = vec3(0,0,0);
      }else{
        glyphManager->set_glyph_color("aabb",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glyphManager->set_glyph_color("selection",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        *backgColor = vec3(1.0f,1.0f,1.0f);
      }
    }

    //Visualization mode
    static bool visualization = false;
    if(ImGui::Checkbox("Visualization", &visualization)){
      if(visualization == true){
        glyphManager->set_visibility("axis", false);
        glyphManager->set_visibility("axiscloud", false);
        glyphManager->set_visibility("grid", false);
        glyphManager->set_visibility("aabb", false);
        *backgColor = vec3(1.0f,1.0f,1.0f);
      }else{
        glyphManager->set_visibility("axis", true);
        glyphManager->set_visibility("axiscloud", true);
        glyphManager->set_visibility("grid", true);
        glyphManager->set_visibility("aabb", true);

        if(darkMode == true){
          *backgColor = vec3(0,0,0);
        }
      }
    }

    //Mouse wheel mode
    int* wheel_mode = gui_controlManager->get_mouseWheelMode();
    ImGui::Text("Mouse wheel");
    ImGui::Combo("##1", wheel_mode, "Frame ID\0Cloud rotation\0");

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_option::option_font(){
  ImGuiIO& io = ImGui::GetIO();
  //---------------------------

  static int font_selected = 0;
  ImGui::PushItemWidth(50);
  if(ImGui::Combo("Font size", &font_selected, "13\0 12\0")){
    ImFont* font = io.Fonts->Fonts[font_selected];
    io.FontDefault = font;
  }

  //---------------------------
  ImGui::Separator();
}
