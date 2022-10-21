#include "GUI.h"
#include "GUI_Control.h"
#include "GUI_Option.h"

#include "../Node_gui.h"
#include "../Box/GUI_windows.h"

#include "../Window/GUI_Consol.h"
#include "../Window/GUI_MenuBar.h"
#include "../Window/GUI_LeftPanel.h"
#include "../Window/struct_consol.h"

#include "../../Engine/OpenGL/Camera/Camera.h"
#include "../../Engine/OpenGL/Dimension.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/Node_engine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"

#include <thread>


//Constructor / Destructor
GUI::GUI(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();

  this->dimManager = node_engine->get_dimManager();
  this->gui_window = node_gui->get_gui_window();
  this->gui_control = node_gui->get_gui_control();
  this->gui_option = node_gui->get_gui_option();
  this->gui_leftPanel = node_gui->get_gui_leftPanel();
  this->gui_menuBar = node_gui->get_gui_menuBar();
  this->gui_consol = node_gui->get_gui_consol();

  this->is_visualization = configManager->parse_json_b("window", "visualization");

  //---------------------------
  this->Gui_init();
  this->Gui_style();
  this->Gui_font();
}
GUI::~GUI(){}

//Main functions
void GUI::Gui_init(){
  //---------------------------

  //ImGui stuff
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(dimManager->get_window(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  //---------------------------
}
void GUI::Gui_style(){
  ImGuiStyle& style = ImGui::GetStyle();
  ImGuiIO& io = ImGui::GetIO();
  //---------------------------

  //Formatting
  style.FrameBorderSize = 0.0f;
  style.WindowPadding.x = 5.0f;

  //Rounding
  style.WindowRounding = 0.0f;
  style.TabRounding = 0.0f;
  style.GrabRounding = 0.0f;
  style.ScrollbarRounding = 0.0f;
  style.ChildRounding = 0.0f;
  style.FrameRounding = 0.0f;
  style.PopupRounding = 0.0f;
  //style.IndentSpacing = 0.0f; //Pas d'indentation des tree

  //Colors
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(112, 112, 112, 127)); //Frame background
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 127)); //Button color
  ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(76, 76, 76, 124));
  ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(117, 117, 117, 220));

  //IO parameters
  io.ConfigWindowsResizeFromEdges = true;

  //---------------------------
}
void GUI::Gui_font(){
  ImGuiIO io = ImGui::GetIO();
  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  ImFontConfig config;
  config.MergeMode = true;
  config.GlyphMinAdvanceX = 15.0f; //Monospace icons
  //---------------------------

  //Load font size 13
  io.Fonts->AddFontFromFileTTF("../media/engine/font/DroidSans.ttf", 13.0f);
  io.Fonts->AddFontFromFileTTF("../media/engine/font/fontawesome-webfont.ttf", 12.5f, &config, icons_ranges);

  //Load font size 12
  io.Fonts->AddFontFromFileTTF("../media/engine/font/DroidSans.ttf", 12.0f);
  io.Fonts->AddFontFromFileTTF("../media/engine/font/fontawesome-webfont.ttf", 12.5f, &config, icons_ranges);

  //Buid the font database
  io.Fonts->Build();

  //---------------------------
}
void GUI::Gui_loop(){
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  //---------------------------

  gui_consol->design_consol();
  gui_menuBar->design_MenuBar();
  gui_control->make_control();
  gui_window->window_Draw();

  if(is_visualization){
    gui_leftPanel->design_leftPanel();
  }

  this->Gui_Dimensions();

  //---------------------------
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void GUI::Gui_render(){
  //---------------------------

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  //---------------------------
}

//Subfunctions
void GUI::Gui_Dimensions(){
  vec2 lbp_dim = gui_leftPanel->get_lbp_dim();
  vec2 bp_dim = gui_consol->get_bp_dim();
  ImGuiIO io = ImGui::GetIO();
  //---------------------------

  //Resing only when mouse dragging
  if(ImGui::IsMouseDragging(0) && io.WantCaptureMouse){
    //dimManager->set_gui_leftPanel_width(lbp_dim.x);
    //dimManager->set_gui_bottomPanel_height(bp_dim.y);
  }

  //---------------------------
}
