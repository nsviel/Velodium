#include "GUI_Option.h"

#include "../GUI_node.h"
#include "../Control/GUI_Control.h"

#include "../../Load/Load_node.h"
#include "../../Load/Processing/Pather.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"
#include "../../Engine/Scene/Object/AABB.h"
#include "../../Engine/Scene/Object/Grid.h"
#include "../../Engine/Scene/Object/Axis.h"
#include "../../Engine/Scene/Object/Normal.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/OpenGL/Camera/Renderer.h"

#include "../../Operation/Operation_node.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Transformation/Transforms.h"

#include "imgui/imgui.h"


//Constructor / Destructor
GUI_option::GUI_option(GUI_node* node_gui){
  //---------------------------

  Engine_node* node_engine = node_gui->get_node_engine();
  Operation_node* node_ope = node_gui->get_node_ope();
  Load_node* node_load = node_engine->get_node_load();

  this->renderManager = node_engine->get_renderManager();
  this->gui_controlManager = node_gui->get_gui_controlManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->attribManager = node_ope->get_attribManager();
  this->pathManager = node_load->get_pathManager();
  this->configManager = node_engine->get_configManager();

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
  static bool grid_ON = true;
  if(ImGui::Checkbox("Grid", &grid_ON)){
    Grid* gridObject = glyphManager->get_object_grid();
    Glyph* grid = gridObject->get_grid();

    grid->visibility = grid_ON;
  }
  ImGui::NextColumn();

  //Subgrid
  static bool grid_sub_ON = false;
  if(ImGui::Checkbox("Subgrid", &grid_sub_ON)){
    Grid* gridObject = glyphManager->get_object_grid();
    Glyph* grid_sub = gridObject->get_grid_sub();
    Glyph* grid_plane = gridObject->get_grid_plane();

    grid_sub->visibility = grid_sub_ON;
    grid_plane->visibility = grid_sub_ON;
  }
  ImGui::NextColumn();

  //Display Bounding Box
  static bool aabb_ON = true;
  if(ImGui::Checkbox("AABB", &aabb_ON)){
    AABB* aabbObject = glyphManager->get_object_aabb();
    Glyph* aabb = aabbObject->get_aabb();

    aabb->visibility = aabb_ON;
  }
  ImGui::NextColumn();

  //Display normals
  static bool normal_ON = false;
  if(ImGui::Checkbox("Normal", &normal_ON)){

  }
  ImGui::NextColumn();

  //Display ICP line correspondences
  static bool matchingON = false;
  if(ImGui::Checkbox("Match", &matchingON)){

  }
  ImGui::NextColumn();

  //Display Axis
  static bool axis_scene_ON = true;
  if(ImGui::Checkbox("Axis", &axis_scene_ON)){
    Axis* axisObject = glyphManager->get_object_axis();
    Glyph* axis_scene = axisObject->get_axis_scene();

    axis_scene->visibility = axis_scene_ON;
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
      if(!sceneManager->get_is_list_empty()){
        heatmapManager->set_Heatmap(cloud);
      }
    }
    ImGui::SameLine();

    //Heatmap all
    static bool heatAll = false;
    if(ImGui::Button("Apply all", ImVec2(75,0))){
      if(!sceneManager->get_is_list_empty()){
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
    vec4* screen_color = renderManager->get_screen_color();
    ImGui::SetNextItemWidth(colorEditSize);
    ImGui::ColorEdit4("Background", (float*)screen_color);

    /*/Normals color
    ImGui::SetNextItemWidth(colorEditSize);
    vec4* color_normals = glyphManager->get_glyph_color("normal");
    if(ImGui::ColorEdit4("Normals", (float*)color_normals)){
      glyphManager->update_glyph_color("normal", *color_normals);
    }*/

    //Grid color
    ImGui::SetNextItemWidth(colorEditSize);
    Grid* gridObject = glyphManager->get_object_grid();
    vec4* grid_color = gridObject->get_grid_color();
    if(ImGui::ColorEdit4("Grid", (float*)grid_color)){
      glyphManager->update_glyph_color(gridObject->get_grid(), *grid_color);
    }

    //Bounding box color
    ImGui::SetNextItemWidth(colorEditSize);
    AABB* aabbObject = glyphManager->get_object_aabb();
    vec4* aabb_color = aabbObject->get_aabb_color();
    if(ImGui::ColorEdit4("AABB", (float*)aabb_color)){
      glyphManager->update_glyph_color(aabbObject->get_aabb(), *aabb_color);
    }

    //Uniform cloud color
    if(cloud != nullptr){
      vec4 cloud_color = cloud->unicolor;

      ImGui::SetNextItemWidth(colorEditSize);
      if(ImGui::ColorEdit4("Point cloud", (float*)&cloud_color, ImGuiColorEditFlags_AlphaBar)){
        if(!sceneManager->get_is_list_empty()){
          attribManager->set_cloud_color(cloud, cloud_color);
        }
      }
    }

    //Uniform subset color
    if(cloud != nullptr){
      Subset* subset = sceneManager->get_subset_selected();
      vec4 subset_color = subset->unicolor;

      ImGui::SetNextItemWidth(colorEditSize);
      if(ImGui::ColorEdit4("Cloud subset", (float*)&subset_color, ImGuiColorEditFlags_AlphaBar)){
        if(!sceneManager->get_is_list_empty()){
          Subset* subset = sceneManager->get_subset_selected();
          attribManager->set_subset_color(subset, subset_color);
        }
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Reset", ImVec2(75,0))){
      float bkg_color = configManager->parse_json_f("window", "background_color");
      vec4* screen_color = renderManager->get_screen_color();
      *screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

      glyphManager->reset_color();
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
      if(!sceneManager->get_is_list_empty()){
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
    ImGui::PushButtonRepeat(true);
    static int point_size = 1;
    if(cloud != nullptr){
      point_size = cloud->point_size;
    }
    if (ImGui::ArrowButton("##left", ImGuiDir_Left) && cloud != nullptr){
      cloud->point_size--;

      if(cloud->point_size <= 1){
        cloud->point_size = 1;
      }
    }
    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right) && cloud != nullptr){
      cloud->point_size++;

      point_size = cloud->point_size;
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", point_size);
    ImGui::NextColumn();

    //Normals size
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Normal size ");
    ImGui::NextColumn();
    static int cpt_nor = 1;
    ImGui::PushButtonRepeat(true);
    if(ImGui::ArrowButton("##left_n", ImGuiDir_Left)){
      cpt_nor--;
      if(cpt_nor <= 1){
        cpt_nor = 1;
      }
      //glyphManager->set_size_normal(cpt_nor);
    }
    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
    if(ImGui::ArrowButton("##right_n", ImGuiDir_Right)){
      cpt_nor++;
      //glyphManager->set_size_normal(cpt_nor);
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
      vec4* screen_color = renderManager->get_screen_color();

      /*if(darkMode == true){
        glyphManager->update_glyph_color("aabb",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glyphManager->update_glyph_color("selection",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        *screen_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }else{
        glyphManager->update_glyph_color("aabb",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glyphManager->update_glyph_color("selection",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        *screen_color = vec4(1.0f,1.0f,1.0f, 1.0f);
      }*/
    }

    //Visualization mode
    static bool visualization = false;
    if(ImGui::Checkbox("Visualization", &visualization)){
      vec4* screen_color = renderManager->get_screen_color();

      /*if(visualization == true){
        glyphManager->set_visibility("axis", false);
        glyphManager->set_visibility("axiscloud", false);
        glyphManager->set_visibility("grid", false);
        glyphManager->set_visibility("aabb", false);
        *screen_color = vec4(1.0f,1.0f,1.0f, 1.0f);
      }else{
        glyphManager->set_visibility("axis", true);
        glyphManager->set_visibility("axiscloud", true);
        glyphManager->set_visibility("grid", true);
        glyphManager->set_visibility("aabb", true);

        if(darkMode == true){
          *screen_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
      }*/
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
