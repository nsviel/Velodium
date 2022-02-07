#include "GUI_MenuBar.h"

#include "GUI_LeftPanel.h"

#include "../Windows/Window_table.h"
#include "../Windows/GUI_windows.h"
#include "../Node/GUI_Option.h"
#include "../GUI_node.h"

#include "../../Load/Operation.h"
#include "../../Module/Module_GUI.h"
#include "../../Module/Player/GUI/GUI_Player.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Data/struct_generic.h"
#include "../../Engine/OpenGL/Textures.h"
#include "../../Engine/Configuration/Configuration_node.h"
#include "../../Engine/Configuration/config_module.h"

#include "../../Operation/Functions/Extraction.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"

#include "imgui/imgui.h"
#include "IconsFontAwesome5.h"

#include <thread>
#include <sys/sysinfo.h>

extern struct Window_tab window_tab;


//Constructor / Destructor
GUI_menuBar::GUI_menuBar(GUI_node* node_gui){
  //---------------------------

  Configuration_node* node_config = node_gui->get_node_config();
  Module_node* node_module = node_gui->get_node_module();
  GUI_module* gui_moduleManager = node_gui->get_gui_moduleManager();

  this->optionManager = node_gui->get_gui_optionManager();
  this->gui_winManager = node_gui->get_gui_winManager();
  this->gui_leftPanelManager = node_gui->get_gui_leftPanelManager();
  this->playerManager = gui_moduleManager->get_gui_playerManager();
  this->configManager = node_config->get_conf_modManager();
  this->sceneManager = new Scene();
  this->heatmapManager = new Heatmap();
  this->texManager = new Textures();
  this->extractionManager = new Extraction();
  this->opeManager = new Operation();

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
      if(sceneManager->is_atLeastOnecloud()){
        Transforms transformManager;
        transformManager.make_centering(cloud);
        //Module* moduleManager = engineManager->get_moduleManager();
        //moduleManager->module_update();
      }
    }
    if(ImGui::MenuItem("Reset scene","r")){
      //engineManager->reset();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Quit")){
      //engineManager->exit();
    }

    ImGui::EndMenu();
  }
  if (ImGui::MenuItem(ICON_FA_FILE " Open")){
    opeManager->loading();
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
    this->MenuBar_fastScene();
    ImGui::EndMenu();
  }

  //-------------------------
}
void GUI_menuBar::MenuBar_icons(){
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(22, 110, 161, 255));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
  float iconSize = 0;
  Texture* texture;
  //---------------------------

  //Cloud
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150);
  if(ImGui::Button(ICON_FA_COMMENT, ImVec2(iconSize,iconSize))){
    if(sceneManager->is_atLeastOnecloud()){
      window_tab.show_modifyFileInfo = !window_tab.show_modifyFileInfo;
    }
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Cloud info");
  }

  //Heatmap
  //ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
  if(ImGui::Button(ICON_FA_EYE, ImVec2(iconSize,iconSize))){
    if(sceneManager->is_atLeastOnecloud()){
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

  playerManager->player_selection();

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
    ImGui::Text("Total RAM  : %5.1f MB\n", si.totalram / megabyte);
    ImGui::Text("Free RAM   : %5.1f MB - %.1f%%\n", si.freeram / megabyte, percentFreeRam);

    //Number of concurrency threads
    const auto processor_count = std::thread::hardware_concurrency();
    ImGui::Text("Max nb thread   : %d\n", processor_count);
    ImGui::Separator();

    //Framerate
    ImGuiIO io = ImGui::GetIO();
    ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    //---------------------------
    ImGui::EndMenu();
  }
}
void GUI_menuBar::MenuBar_fastScene(){
  //---------------------------

  //Two Buddha point cloud to register
  if(ImGui::Button("Buddha", ImVec2(100,0))){
    opeManager->loading_fastScene(0);
  }
  //Two Torus point cloud to register
  if(ImGui::Button("Torus", ImVec2(100,0))){
    opeManager->loading_fastScene(1);
  }
  if(ImGui::Button("PCAP file", ImVec2(100,0))){
    opeManager->loading_fastScene(2);
  }
  if(ImGui::Button("Frames move", ImVec2(100,0))){
    opeManager->loading_fastScene(3);
    configManager->choose_configuration(0);
    //gui_leftPanelManager->update();
  }
  if(ImGui::Button("More frames", ImVec2(100,0))){
    opeManager->loading_fastScene(4);
    configManager->choose_configuration(0);
    //gui_leftPanelManager->update();
  }
  if(ImGui::Button("Other frames", ImVec2(100,0))){
    opeManager->loading_fastScene(5);
    configManager->choose_configuration(0);
    //gui_leftPanelManager->update();
  }
  if(ImGui::Button("Tunel", ImVec2(100,0))){
    opeManager->loading_fastScene(6);
    configManager->choose_configuration(0);
    //gui_leftPanelManager->update();
  }
  if(ImGui::Button("IA module", ImVec2(100,0))){
    opeManager->loading_fastScene(7);
    configManager->choose_configuration(1);
    //gui_leftPanelManager->update();
  }

  //---------------------------
}
