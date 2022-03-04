#include "GUI_MenuBar.h"

#include "GUI_LeftPanel.h"

#include "../Windows/Window_table.h"
#include "../Windows/GUI_windows.h"
#include "../Control/GUI_Option.h"
#include "../Control/GUI_Initialization.h"
#include "../GUI_node.h"

#include "../../Load/Load_node.h"
#include "../../Load/Processing/Pather.h"
#include "../../Module/Module_GUI.h"
#include "../../Module/Player/GUI/GUI_Dynamic.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Data/struct_generic.h"
#include "../../Engine/OpenGL/CoreGLengine.h"
#include "../../Engine/OpenGL/Textures.h"

#include "../../Operation/Operation_node.h"
#include "../../Operation/Functions/Extraction.h"
#include "../../Operation/Transformation/Transforms.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

#include <thread>
#include <sys/sysinfo.h>

extern struct Window_tab window_tab;


//Constructor / Destructor
GUI_menuBar::GUI_menuBar(GUI_node* node){
  this->node_gui = node;
  //---------------------------

  Engine_node* node_engine = node_gui->get_node_engine();
  Module_node* node_module = node_gui->get_node_module();
  GUI_module* gui_moduleManager = node_gui->get_gui_moduleManager();
  Operation_node* node_ope = node_gui->get_node_ope();
  GUI_Player* gui_player = gui_moduleManager->get_gui_playerManager();
  Load_node* node_load = node_engine->get_node_load();

  this->gui_init = node_gui->get_gui_initManager();
  this->optionManager = node_gui->get_gui_optionManager();
  this->gui_winManager = node_gui->get_gui_winManager();
  this->gui_leftPanelManager = node_gui->get_gui_leftPanelManager();
  this->gui_dynamic = gui_player->get_gui_dynamic();
  this->sceneManager = node_engine->get_sceneManager();
  this->extractionManager = node_ope->get_extractionManager();
  this->pathManager = node_load->get_pathManager();
  this->glManager = node_engine->get_glManager();
  this->texManager = new Textures();

  //---------------------------
}
GUI_menuBar::~GUI_menuBar(){}

//Main function
void GUI_menuBar::design_MenuBar(){
  if(ImGui::BeginMainMenuBar()){
    //------------------------

    this->MenuBar_menus();
    this->MenuBar_icons();
    this->MenuBar_subsetSelection();
    this->MenuBar_appInfo();

    //-------------------------
    ImGui::EndMainMenuBar();
  }
}

//Subfunctions
void GUI_menuBar::MenuBar_menus(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //-------------------------

  if (ImGui::BeginMenu("File")){
    if (ImGui::MenuItem("Open options", "o")){
      window_tab.show_loading = true;
    }
    if (ImGui::MenuItem("Save options")){
      window_tab.show_saving = true;
    }
    if(ImGui::MenuItem("Remove cloud", "Suppr")){
      sceneManager->remove_cloud(cloud);
    }
    if(ImGui::MenuItem("Remove all")){
      sceneManager->remove_cloud_all();
    }
    if(ImGui::MenuItem("Next cloud","tab")){
      sceneManager->selection_setNext();
    }
    if(ImGui::MenuItem("Center cloud")){
      if(!sceneManager->get_is_list_empty()){
        Transforms transformManager;
        transformManager.make_centering(cloud);
        //Module* moduleManager = engineManager->get_moduleManager();
        //moduleManager->module_update();
      }
    }
    if(ImGui::MenuItem("Reset scene","r")){
      node_gui->reset();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Quit")){
      node_gui->exit();
    }

    ImGui::EndMenu();
  }
  if (ImGui::MenuItem(ICON_FA_FILE " Open")){
    pathManager->loading();
  }
  if (ImGui::BeginMenu(ICON_FA_COG " Option")){
    optionManager->design_Options();
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Operation")){
    this->MenuBar_Operations();
    ImGui::EndMenu();
  }
  if(ImGui::BeginMenu("Init")){
    gui_init->init_gui();
    ImGui::EndMenu();
  }

  //-------------------------
}
void GUI_menuBar::MenuBar_icons(){
  //---------------------------

  //Distance from left
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);

  //Parameters
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(22, 110, 161, 255));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
  float iconSize = 0;
  Texture* texture;

  //Cloud info
  if(ImGui::Button(ICON_FA_COMMENT, ImVec2(iconSize,iconSize))){
    if(!sceneManager->get_is_list_empty()){
      window_tab.show_modifyFileInfo = !window_tab.show_modifyFileInfo;
    }
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Cloud info");
  }

  //Heatmap
  //ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
  if(ImGui::Button(ICON_FA_EYE, ImVec2(iconSize,iconSize))){
    if(!sceneManager->get_is_list_empty()){
      //Heatmap window
      window_tab.show_heatmap = !window_tab.show_heatmap;
    }
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Heatmap");
  }

  //Camera
  //ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 2);
  if(ImGui::Button(ICON_FA_CAMERA, ImVec2(iconSize,iconSize))){
    window_tab.show_camera = !window_tab.show_camera;
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Camera");
  }

  //---------------------------
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(1);
}
void GUI_menuBar::MenuBar_subsetSelection(){
  //-------------------------

  //Distance from left
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);

  gui_dynamic->player_button();
  ImGui::SameLine();
  gui_dynamic->player_selection();

  //-------------------------
}
void GUI_menuBar::MenuBar_Operations(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Functions
  if(ImGui::Button("Shader", ImVec2(150,0))){
    window_tab.show_shader = !window_tab.show_shader;
  }
  if(ImGui::Button("Filter", ImVec2(150,0))){
    window_tab.show_filter = !window_tab.show_filter;
  }
  if(ImGui::Button("Intensity", ImVec2(150,0))){
    window_tab.show_intensity = !window_tab.show_intensity;
  }
  if(ImGui::Button("Color", ImVec2(150,0))){
    window_tab.show_color = !window_tab.show_color;
  }
  if(ImGui::Button("Normal", ImVec2(150,0))){
    window_tab.show_normal = !window_tab.show_normal;
  }
  if(ImGui::Button("Extract cloud", ImVec2(150,0))){
    window_tab.show_extractCloud = !window_tab.show_extractCloud;
  }
  if(ImGui::Button("Cut cloud", ImVec2(150,0))){
    window_tab.show_cutCloud = !window_tab.show_cutCloud;
  }
  if(ImGui::Button("Transformation", ImVec2(150,0))){
    window_tab.show_transformation = !window_tab.show_transformation;
  }

  if(ImGui::CollapsingHeader("Modules")){

  }

  //---------------------------
}
void GUI_menuBar::MenuBar_appInfo(){
  ImGui::SameLine(ImGui::GetWindowWidth()-50);
  if(ImGui::BeginMenu("Infos")){
    //---------------------------

    //OpenGl version
    const char* oglv = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    ImGui::MenuItem("OGL v.", oglv);
    ImGui::Separator();

    //RAM memory
    const double megabyte = 1024 * 1024;
    struct sysinfo si;
    sysinfo (&si);
    float percentFreeRam = ((float)si.freeram*100) / (float)si.totalram;
    ImGui::Text("Total RAM: %5.1f MB\n", si.totalram / megabyte);
    ImGui::Text("Free RAM: %5.1f MB - %.1f%%\n", si.freeram / megabyte, percentFreeRam);

    //Number of concurrency threads
    const auto processor_count = std::thread::hardware_concurrency();
    ImGui::Text("Max nb thread: %d\n", processor_count);
    ImGui::Separator();

    //Framerate
    ImGuiIO io = ImGui::GetIO();
    ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    //Loop duration
    float time_loop = glManager->get_time_loop();
    ImGui::Text("Loop: %.f ms", time_loop);

    //---------------------------
    ImGui::EndMenu();
  }
}
