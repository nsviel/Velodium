#include "GUI_MenuBar.h"
#include "GUI_LeftPanel.h"

#include "../Node_gui.h"
#include "../Modal/Modal_tab.h"
#include "../Modal/GUI_Modal.h"
#include "../Control/GUI_Option.h"
#include "../Control/GUI_Initialization.h"
#include "../Dynamic/GUI_Player.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Pather.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Data/Graph.h"
#include "../../Module/Registration/struct_misc.h"
#include "../../Engine/OpenGL/CoreGLengine.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Cloud/Extraction.h"
#include "../../Operation/Transformation/Pose.h"

#include "../../Specific/Function/fct_math.h"

#include "imgui/imgui.h"
#include "image/IconsFontAwesome5.h"

#include <thread>
#include <sys/sysinfo.h>

extern struct Modal_tab modal_tab;


//Constructor / Destructor
GUI_menuBar::GUI_menuBar(Node_gui* node){
  this->node_gui = node;
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_module* node_module = node_gui->get_node_module();
  Node_operation* node_ope = node_gui->get_node_ope();
  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->gui_init = node_gui->get_gui_initialization();
  this->optionManager = node_gui->get_gui_option();
  this->gui_window = node_gui->get_gui_window();
  this->gui_leftPanel = node_gui->get_gui_leftPanel();
  this->gui_player = node_gui->get_gui_player();
  this->sceneManager = node_scene->get_sceneManager();
  this->extractionManager = node_ope->get_extractionManager();
  this->pathManager = node_load->get_patherManager();
  this->glManager = node_engine->get_glManager();
  this->graphManager = node_scene->get_graphManager();

  //---------------------------
}
GUI_menuBar::~GUI_menuBar(){}

//Main function
void GUI_menuBar::design_MenuBar(){
  if(ImGui::BeginMainMenuBar()){
    //------------------------

    this->MenuBar_menu();
    this->MenuBar_icons();
    this->MenuBar_subsetSelection();
    this->MenuBar_appInfo();

    //-------------------------
    ImGui::EndMainMenuBar();
  }
}

//Subfunctions
void GUI_menuBar::MenuBar_menu(){
  Collection* collection = sceneManager->get_selected_collection();
  //-------------------------

  this->MenuBar_menu_file();
  this->MenuBar_menu_load();
  this->MenuBar_menu_save();

  //Option menu
  if(ImGui::BeginMenu(ICON_FA_COG, " Option")){
    optionManager->design_Options();
    ImGui::EndMenu();
  }

  //Operation menu
  if(ImGui::BeginMenu("Operation")){
    this->MenuBar_Operations();
    ImGui::EndMenu();
  }

  //Init menu
  if(ImGui::BeginMenu("Init")){
    gui_init->init_gui();
    ImGui::EndMenu();
  }

  //-------------------------
}
void GUI_menuBar::MenuBar_menu_file(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //File menu
  if(ImGui::BeginMenu("File")){
    if(ImGui::MenuItem("Remove collection", "Suppr")){
      sceneManager->remove_collection(collection);
    }
    if(ImGui::MenuItem("Remove all", "Fin")){
      sceneManager->remove_collection_all();
    }
    if(ImGui::MenuItem("Next collection","tab")){
      graphManager->select_next_collection();
    }
    if(ImGui::MenuItem("Center collection", "c")){
      if(!sceneManager->get_is_list_empty()){
        Pose poseManager;
        poseManager.make_centering(collection);
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

  //---------------------------
}
void GUI_menuBar::MenuBar_menu_load(){
  Collection* collection = sceneManager->get_selected_collection();
  //-------------------------

  if(ImGui::BeginMenu(ICON_FA_FILE, "Open")){
    if (ImGui::MenuItem("Open")){
      pathManager->loading_cloud();
    }
    if (ImGui::MenuItem("Open frames")){
      pathManager->loading_frame();
    }
    if (ImGui::MenuItem("Open on-the-fly")){
      pathManager->loading_onthefly();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Options")){
      modal_tab.show_loading = true;
    }
    ImGui::EndMenu();
  }

  //-------------------------
}
void GUI_menuBar::MenuBar_menu_save(){
  Collection* collection = sceneManager->get_selected_collection();
  //-------------------------

  if(ImGui::BeginMenu(ICON_FA_BOOK, "Save")){
    if(ImGui::MenuItem("Save")){
      pathManager->saving_cloud_same(collection);
    }
    if(ImGui::MenuItem("Save collection")){
      pathManager->saving_cloud(collection);
    }
    if(ImGui::MenuItem("Save frame")){
      pathManager->saving_cloud_frame(collection);
    }
    ImGui::Separator();
    if(ImGui::MenuItem("Options")){
      modal_tab.show_saving = true;
    }
    ImGui::EndMenu();
  }

  //-------------------------
}
void GUI_menuBar::MenuBar_icons(){
  //---------------------------

  //Distance from left
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);

  //Parameters
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(22, 110, 161, 255));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
  float iconSize = 0;
  Texture* texture;

  //Collection info
  if(ImGui::Button(ICON_FA_COMMENT, ImVec2(iconSize,iconSize))){
    if(!sceneManager->get_is_list_empty()){
      modal_tab.show_modifyFileInfo = !modal_tab.show_modifyFileInfo;
    }
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Collection info");
  }

  //Heatmap
  if(ImGui::Button(ICON_FA_EYE, ImVec2(iconSize,iconSize))){
    if(!sceneManager->get_is_list_empty()){
      //Heatmap window
      modal_tab.show_color = !modal_tab.show_color;
    }
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Colorization");
  }

  //Camera
  if(ImGui::Button(ICON_FA_CAMERA, ImVec2(iconSize,iconSize))){
    modal_tab.show_camera = !modal_tab.show_camera;
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Camera");
  }

  //Boxing
  if(ImGui::Button(ICON_FA_CUBE, ImVec2(iconSize,iconSize))){
    modal_tab.show_boxing = !modal_tab.show_boxing;
  }
  if(ImGui::IsItemHovered()){
    ImGui::SetTooltip("Collection boxing");
  }

  //---------------------------
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(1);
}
void GUI_menuBar::MenuBar_subsetSelection(){
  //-------------------------

  //Distance from left
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);

  gui_player->player_button();
  ImGui::SameLine();
  gui_player->player_selection();

  //-------------------------
}
void GUI_menuBar::MenuBar_Operations(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //Functions
  if(ImGui::Button("Shader", ImVec2(150,0))){
    modal_tab.show_shader = !modal_tab.show_shader;
  }
  if(ImGui::Button("Filter", ImVec2(150,0))){
    modal_tab.show_filter = !modal_tab.show_filter;
  }
  if(ImGui::Button("Intensity", ImVec2(150,0))){
    modal_tab.show_intensity = !modal_tab.show_intensity;
  }
  if(ImGui::Button("Color", ImVec2(150,0))){
    modal_tab.show_color = !modal_tab.show_color;
  }
  if(ImGui::Button("Normal", ImVec2(150,0))){
    modal_tab.show_normal = !modal_tab.show_normal;
  }
  if(ImGui::Button("Cutting tools", ImVec2(150,0))){
    modal_tab.show_extractCloud = !modal_tab.show_extractCloud;
  }
  if(ImGui::Button("Transformation", ImVec2(150,0))){
    modal_tab.show_transformation = !modal_tab.show_transformation;
  }
  if(ImGui::Button("Texture", ImVec2(150,0))){
    modal_tab.show_texture = !modal_tab.show_texture;
  }

  //---------------------------
}
void GUI_menuBar::MenuBar_appInfo(){
  ImGui::SameLine(ImGui::GetWindowWidth()-50);
  if(ImGui::BeginMenu("Infos")){
    //---------------------------

    //OpenGl version
    const char* oglv = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    ImGui::Text("OGL v.");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.7f,0.7f,0.7f,1.0f), "%s", oglv);
    ImGui::Separator();

    //RAM memory
    const double megabyte = 1024 * 1024;
    struct sysinfo si;
    sysinfo (&si);
    float percentFreeRam = ((float)si.freeram*100) / (float)si.totalram;
    string total_ram = thousandSeparator(si.totalram / megabyte);
    ImGui::Text("Total RAM: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%s", total_ram.c_str());
    ImGui::SameLine();
    ImGui::Text(" MB\n");

    ImGui::Text("Free RAM: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%5.1f", si.freeram / megabyte);
    ImGui::SameLine();
    ImGui::Text(" MB [");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%.1f", percentFreeRam);
    ImGui::SameLine();
    ImGui::Text("%% ]");

    //Framerate
    ImGuiIO io = ImGui::GetIO();
    ImGui::Text("Display: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%.1f", 1000.0f / io.Framerate);
    ImGui::SameLine();
    ImGui::Text(" ms/frame [");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%.1f", io.Framerate);
    ImGui::SameLine();
    ImGui::Text(" FPS ]");

    //Loop duration
    float time_loop = glManager->get_time_loop();
    ImGui::Text("Loop: ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "%.f", time_loop);
    ImGui::SameLine();
    ImGui::Text(" ms");

    //---------------------------
    ImGui::EndMenu();
  }
}
