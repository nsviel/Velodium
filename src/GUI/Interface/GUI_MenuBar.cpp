#include "GUI_MenuBar.h"
#include "GUI_LeftPanel.h"

#include "../Windows/Window_tab.h"
#include "../Windows/GUI_windows.h"
#include "../Node/GUI_Option.h"

#include "../../Module/CloudPlayer/GUI_CloudPlayer.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Data/struct_generic.h"
#include "../../Engine/OpenGL/Textures.h"

#include "../../Operation/Operation.h"
#include "../../Operation/Functions/Extraction.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Transformation/Transforms.h"

#include "../../../extern/imgui/imgui.h"
#include "../../../extern/IconsFontAwesome5.h"

#include <sys/sysinfo.h>

extern struct Window_tab window_tab;


//Constructor / Destructor
GUI_menuBar::GUI_menuBar(Engine* engine, GUI_windows* winManager, GUI_option* options, GUI_leftPanel* leftPanel){
  this->optionManager = options;
  this->gui_winManager = winManager;
  this->engineManager = engine;
  this->gui_leftPanelManager = leftPanel;
  //---------------------------

  this->sceneManager = new Scene();
  this->heatmapManager = new Heatmap();
  this->texManager = new Textures();
  this->extractionManager = new Extraction();
  this->opeManager = new Operation();
  this->playerManager = new GUI_CloudPlayer(engineManager->get_CameraManager());

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
      sceneManager->removeCloud(cloud);
    }
    if(ImGui::MenuItem("Remove all")){
      sceneManager->removeCloud_all();
    }
    if(ImGui::MenuItem("Next cloud","tab")){
      sceneManager->selection_setNext();
    }
    if(ImGui::MenuItem("Center cloud")){
      if(sceneManager->is_atLeastOnecloud()){
        Transforms transformManager;
        transformManager.make_centering(cloud);
        sceneManager->update_cloud_location(cloud);
      }
    }
    if(ImGui::MenuItem("Reset scene","r")){
      opeManager->reset();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Quit")){
      engineManager->Exit();
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

  playerManager->subset_selection_bar();

  //-------------------------
}
void GUI_menuBar::MenuBar_fastScene(){
  //---------------------------

  //Two Buddha point cloud to register
  if(ImGui::Button("Buddha", ImVec2(100,0))){
    opeManager->fastScene(0);
  }
  //Two Torus point cloud to register
  if(ImGui::Button("Torus", ImVec2(100,0))){
    opeManager->fastScene(1);
  }
  if(ImGui::Button("PCAP file", ImVec2(100,0))){
    opeManager->fastScene(2);
  }
  if(ImGui::Button("Frames move", ImVec2(100,0))){
    opeManager->fastScene(3);
  }
  if(ImGui::Button("More frames", ImVec2(100,0))){
    opeManager->fastScene(4);
  }
  if(ImGui::Button("Scala", ImVec2(100,0))){
    opeManager->fastScene(5);
  }

  //---------------------------
}
void GUI_menuBar::MenuBar_Operations(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Functions
  if(ImGui::Button("Shader", ImVec2(150,0))){
    window_tab.show_shader = !window_tab.show_shader;
  }
  if(ImGui::Button("Filter", ImVec2(150,0))){
    window_tab.show_filtering = !window_tab.show_filtering;
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
    ImGui::Text("total RAM   : %5.1f MB\n", si.totalram / megabyte);
    ImGui::Text("free RAM   : %5.1f MB - %.1f%%\n", si.freeram / megabyte, percentFreeRam);
    ImGui::Separator();

    //Framerate
    ImGuiIO io = ImGui::GetIO();
    ImGui::Text("%.1f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    //---------------------------
    ImGui::EndMenu();
  }
}
