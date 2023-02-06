#include "GUI_Option.h"

#include "../Node_gui.h"
#include "../Control/GUI_Control.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Pather.h"

#include "../../Engine/Core/Engine.h"
#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyph/Object.h"
#include "../../Engine/Scene/Glyph/Cloud/AABB.h"
#include "../../Engine/Scene/Glyph/Scene/Grid.h"
#include "../../Engine/Scene/Glyph/Scene/Axis.h"
#include "../../Engine/Scene/Glyph/Cloud/Normal.h"
#include "../../Engine/Scene/Glyph/Cloud/Tree.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/OpenGL/Renderer.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Color/Heatmap.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Transformation.h"

#include "imgui/imgui.h"


//Constructor / Destructor
GUI_option::GUI_option(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_operation* node_ope = node_gui->get_node_ope();
  Node_load* node_load = node_engine->get_node_load();

  this->renderManager = node_engine->get_renderManager();
  this->gui_control = node_gui->get_gui_control();
  this->objectManager = node_engine->get_objectManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->colorManager = node_ope->get_colorManager();
  this->pathManager = node_load->get_patherManager();
  this->configManager = node_engine->get_configManager();

  //---------------------------
}
GUI_option::~GUI_option(){}

//Main function
void GUI_option::design_Options(){
  //---------------------------

  this->option_font();
  this->option_glyph();
  this->option_mode();
  this->option_parameter();
  this->option_color();

  //---------------------------
}

//Subfunctions
void GUI_option::option_font(){
  ImGuiIO& io = ImGui::GetIO();
  //---------------------------

  static int font_selected = 0;
  ImGui::SetNextItemWidth(150);
  if(ImGui::Combo("Font size", &font_selected, "13\0 12\0")){
    ImFont* font = io.Fonts->Fonts[font_selected];
    io.FontDefault = font;
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_option::option_glyph(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  ImGui::Columns(2);
  //---------------------------

  //Display grid
  Grid* gridObject = objectManager->get_object_grid();
  Glyph* grid = gridObject->get_grid();
  bool& grid_ON = grid->visibility;
  ImGui::Checkbox("Grid", &grid_ON);
  ImGui::NextColumn();

  //Grid number of square
  static int nb_square = 4;
  ImGui::PushItemWidth(75);
  if(ImGui::DragInt("##457", &nb_square, 1, 1, 100, "%d")){
    Grid* gridObject = objectManager->get_object_grid();
    gridObject->update_grid(nb_square);
    gridObject->update_grid_sub(nb_square);
    gridObject->update_grid_plane(nb_square);
    objectManager->update_object(gridObject->get_grid());
    objectManager->update_object(gridObject->get_grid_sub());
    objectManager->update_object(gridObject->get_grid_plane());
  }
  ImGui::NextColumn();

  //Subgrid
  Glyph* grid_sub = gridObject->get_grid_sub();
  bool& grid_sub_ON = grid_sub->visibility;
  if(ImGui::Checkbox("Subgrid", &grid_sub_ON)){
    Glyph* grid_plane = gridObject->get_grid_plane();
    grid_plane->visibility = grid_sub_ON;
  }
  ImGui::NextColumn();

  //Display Bounding Box
  AABB* aabbObject = objectManager->get_object_aabb();
  Glyph* aabb = aabbObject->get_glyph();
  bool& aabb_ON = aabb->visibility;
  ImGui::Checkbox("AABB", &aabb_ON);
  ImGui::NextColumn();

  //Display normals
  Normal* normObject = objectManager->get_object_normal();
  bool* norm_visib = normObject->get_visibility();
  if(ImGui::Checkbox("Normal", norm_visib)){
    if(sceneManager->get_is_list_empty() == false){
      objectManager->set_object_visibility("normal", *norm_visib);
    }
  }
  ImGui::NextColumn();

  //RAJOUTER UN TRUC ICI
  ImGui::NextColumn();

  //Display Axis world
  Axis* axisObject = objectManager->get_object_axis();
  Glyph* axis_scene = axisObject->get_axis_scene();
  bool& axis_scene_ON = axis_scene->visibility;
  ImGui::Checkbox("Axis world", &axis_scene_ON);
  ImGui::NextColumn();

  //Display Axis cloud
  static bool axis_cloud = true;
  if(ImGui::Checkbox("Axis cloud", &axis_cloud)){
    bool* axis_visibility = axisObject->get_axis_subset_visibility();
    *axis_visibility = axis_cloud;
    objectManager->update_glyph_cloud(cloud);
  }
  ImGui::NextColumn();

  //Display Axis circle
  static bool axisCircleON = false;
  if(ImGui::Checkbox("Axis circle", &axisCircleON)){
    //objectManager->obj_axisCircle(circleRadius);
    //objectManager->set_visibility("axisCircle", axisCircleON);
  }
  ImGui::NextColumn();

  //Axis circlev radius
  static float circleRadius = 1;
  ImGui::PushItemWidth(75);
  if(ImGui::DragFloat("##456", &circleRadius, 0.001, 0, 5, "%.3f")){
    //objectManager->obj_axisCircle(circleRadius);
  }
  ImGui::NextColumn();

  //Display tree
  Tree* treeObject = objectManager->get_object_tree();
  bool* tree_visible = treeObject->get_visibility();
  if(ImGui::Checkbox("Tree", tree_visible)){
    if(sceneManager->get_is_list_empty() == false){
      objectManager->set_object_visibility("tree", *tree_visible);
    }
  }
  ImGui::NextColumn();

  //Tree level
  int* tree_level = treeObject->get_tree_level();
  ImGui::PushItemWidth(75);
  if(ImGui::DragInt("##458", tree_level, 1, 1, 50, "%d")){
    Subset* subset = cloud->subset_selected;
    treeObject->update_tree(subset);
    objectManager->update_object(&subset->glyphs["tree"]);
  }
  ImGui::NextColumn();

  //---------------------------
  ImGui::Columns(1);
  ImGui::Separator();
}
void GUI_option::option_mode(){
  if(ImGui::CollapsingHeader("Mode")){
    //---------------------------

    //Light / Dark mode
    static bool darkMode = false;
    static vec4 color_old;
    if(ImGui::Checkbox("Dark mode", &darkMode)){
      vec4* screen_color = renderManager->get_screen_color();

      if(darkMode == true){
        color_old = *screen_color;
        objectManager->update_object("aabb",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        objectManager->update_object("selection",vec4(1.0f, 1.0f, 1.0f, 1.0f));
        *screen_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }else{
        objectManager->update_object("aabb",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        objectManager->update_object("selection",vec4(0.0f, 0.0f, 0.0f, 1.0f));
        *screen_color = color_old;
      }
    }

    //Visualization mode
    static bool visualization = false;
    if(ImGui::Checkbox("Display mode", &visualization)){
      vec4* screen_color = renderManager->get_screen_color();
      Glyph* axis = objectManager->get_glyph_by_name("axis");
      Glyph* aabb = objectManager->get_glyph_by_name("aabb");
      Glyph* grid = objectManager->get_glyph_by_name("grid");

      if(visualization == true){
        axis->visibility = false;
        aabb->visibility = false;
        grid->visibility = false;
      }else{
        axis->visibility = true;
        aabb->visibility = true;
        grid->visibility = true;
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_option::option_parameter(){
  if(ImGui::CollapsingHeader("Parameters")){
    Cloud* cloud = sceneManager->get_selected_cloud();
    ImGuiStyle& style = ImGui::GetStyle();
    //---------------------------

    //cloud movement
    float* transCoef = gui_control->get_transCoef();
    ImGui::DragFloat("Translation", transCoef, 0.001, 0, 100, "%.4f m");
    float* rotatDegree = gui_control->get_rotatDegree();
    ImGui::DragFloat("Rotation", rotatDegree, 0.5, 0, 90, "%.4f °");

    //Point cloud scaling
    static float scale = 1.0f;
    ImGui::SetNextItemWidth(75);
    if(ImGui::DragFloat("Scale", &scale, 0.01, 0.1, 100, "%.2f x")){
      if(!sceneManager->get_is_list_empty()){
        Transformation transformManager;
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

    //Draw type
    static int draw_type = 0;
    if(cloud != nullptr){
      if(cloud->draw_type == "point"){
        draw_type = 0;
      }
      if(cloud->draw_type == "line"){
        draw_type = 1;
      }
      if(cloud->draw_type == "triangle"){
        draw_type = 2;
      }
      if(cloud->draw_type == "quad"){
        draw_type = 3;
      }
    }
    if(ImGui::Combo("Draw type", &draw_type, "Point\0Line\0Triangle\0Quad\0")){
      if(draw_type == 0){
        cloud->draw_type = "point";
      }
      else if(draw_type == 1){
        cloud->draw_type = "line";
      }
      else if(draw_type == 2){
        cloud->draw_type = "triangle";
      }
      else if(draw_type == 3){
        cloud->draw_type = "quad";
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
    Normal* normObject = objectManager->get_object_normal();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Normal size ");
    ImGui::NextColumn();
    static int cpt_nor = 1;
    ImGui::PushButtonRepeat(true);
    if(ImGui::ArrowButton("##left_n", ImGuiDir_Left)){
      //Change normal size value
      cpt_nor--;
      if(cpt_nor <= 1){
        cpt_nor = 1;
      }

      //Apply new normal size value
      int* size = normObject->get_size();
      *size = cpt_nor;
      objectManager->update_glyph_cloud(cloud);
    }
    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
    if(ImGui::ArrowButton("##right_n", ImGuiDir_Right)){
      //Change normal size value
      cpt_nor++;

      //Apply new normal size value
      int* size = normObject->get_size();
      *size = cpt_nor;
      objectManager->update_glyph_cloud(cloud);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", cpt_nor);
    ImGui::Columns(1);

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_option::option_color(){
  if(ImGui::CollapsingHeader("Colors")){
    Cloud* cloud = sceneManager->get_selected_cloud();
    int colorEditSize = 150;
    //---------------------------

    //Background color
    vec4* screen_color = renderManager->get_screen_color();
    ImGui::SetNextItemWidth(colorEditSize);
    ImGui::ColorEdit4("Background", (float*)screen_color);

    /*/Normals color
    ImGui::SetNextItemWidth(colorEditSize);
    vec4* color_normals = objectManager->get_glyph_color("normal");
    if(ImGui::ColorEdit4("Normals", (float*)color_normals)){
      objectManager->update_object("normal", *color_normals);
    }*/

    //Grid color
    ImGui::SetNextItemWidth(colorEditSize);
    Grid* gridObject = objectManager->get_object_grid();
    vec4* grid_color = gridObject->get_grid_color();
    if(ImGui::ColorEdit4("Grid", (float*)grid_color)){
      objectManager->update_object(gridObject->get_grid(), *grid_color);
    }

    //Bounding box color
    ImGui::SetNextItemWidth(colorEditSize);
    AABB* aabbObject = objectManager->get_object_aabb();
    vec4* aabb_color = aabbObject->get_glyph_color();
    if(ImGui::ColorEdit4("AABB", (float*)aabb_color)){
      objectManager->update_object(aabbObject->get_glyph(), *aabb_color);
    }

    //Uniform cloud color
    if(cloud != nullptr){
      vec4 cloud_color = cloud->unicolor;

      ImGui::SetNextItemWidth(colorEditSize);
      if(ImGui::ColorEdit4("Point cloud", (float*)&cloud_color, ImGuiColorEditFlags_AlphaBar)){
        if(!sceneManager->get_is_list_empty()){
          colorManager->set_color_new(cloud, cloud_color);
        }
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Reset", ImVec2(75,0))){
      float bkg_color = configManager->parse_json_f("window", "background_color");
      vec4* screen_color = renderManager->get_screen_color();
      *screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

      objectManager->reset_color_object();
    }
  }
}
