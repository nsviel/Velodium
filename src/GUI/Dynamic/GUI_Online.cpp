#include "GUI_Online.h"
#include "GUI_Player.h"

#include "../Node_gui.h"
#include "../Control/GUI_Color.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Online.h"
#include "../../Operation/Dynamic/Player.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Engine/Camera/Followup.h"
#include "../../Engine/GPU/GPU_screenshot.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"

#include "../../Interface/Node_interface.h"
#include "../../Specific/File/Path.h"
#include "../../Interface/IO/Recorder.h"
#include "../../Module/Node_module.h"

#include "imgui/imgui.h"
#include "image/IconsFontAwesome5.h"


//Constructor / Destructor
GUI_Online::GUI_Online(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_interface* node_interface = node_gui->get_node_interface();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->node_engine = node_gui->get_node_engine();
  this->node_module = node_engine->get_node_module();

  this->filterManager = node_ope->get_filterManager();
  this->onlineManager = node_ope->get_onlineManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->followManager = node_engine->get_followManager();
  this->recordManager = node_interface->get_recordManager();
  this->screenshotManager = node_engine->get_gpu_screenshot();
  this->colorManager = node_ope->get_colorManager();
  this->configManager = node_engine->get_configManager();
  this->playerManager = node_ope->get_playerManager();

  this->gui_color = node_gui->get_gui_color();
  this->gui_player = node_gui->get_gui_player();

  this->item_width = 100;

  //---------------------------
}
GUI_Online::~GUI_Online(){}

//Main function
void GUI_Online::design_dynamic(){
  if(ImGui::BeginTabItem("Dynamic")){
    if(ImGui::BeginTabBar("##tabs_dynamic", ImGuiTabBarFlags_None)){
      //---------------------------

      gui_player->design_player();
      this->design_online();
      this->design_state();

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void GUI_Online::design_state(){
  if(ImGui::BeginTabItem("State")){
    //---------------------------

    this->state_configuration();
    this->state_online();
    this->state_time();

    //---------------------------
    ImGui::EndTabItem();
  }
}
void GUI_Online::design_online(){
  if(ImGui::BeginTabItem("Online")){
    //---------------------------

    this->parameter_online();
    this->parameter_filter();
    this->parameter_recorder();
    gui_color->colorization_choice();

    //---------------------------
    ImGui::EndTabItem();
  }
}

//Parameter function
void GUI_Online::parameter_online(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Module online stuff
  node_module->draw_online();

  //Camera followup stuff
  this->parameter_camera();

  //---------------------------
}
void GUI_Online::parameter_filter(){
  //---------------------------

  bool* sphere_filter = onlineManager->get_with_sphere_filter();
  ImGui::Checkbox("Sphere filter", sphere_filter);
  if(*sphere_filter){
    float* r_min = filterManager->get_sphere_min();
    float* r_max = filterManager->get_sphere_max();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
  }

  //---------------------------
}
void GUI_Online::parameter_camera(){
  //---------------------------

  //Camera auto displacement
  bool* with_camera_follow = followManager->get_with_camera_follow();
  bool* with_camera_absolu = followManager->get_with_camera_absolute();
  bool* with_camera_top = followManager->get_with_camera_top();
  ImGui::Checkbox("Camera follow up", with_camera_follow);

  //Relative or absolute
  if(*with_camera_follow){
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    static int view;
    if(*with_camera_absolu){
      view = 1;
    }else{
      view = 0;
    }
    if(ImGui::RadioButton("Relative", &view, 0)){
      *with_camera_absolu = false;
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("Absolute", &view, 1)){
      *with_camera_absolu = true;
    }
  }

  //Oblique or top view
  if(*with_camera_follow && *with_camera_absolu){
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    static int view;
    if(*with_camera_top){
      view = 1;
    }else{
      view = 0;
    }
    if(ImGui::RadioButton("Oblique##444", &view, 0)){
      *with_camera_top = false;
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("Top##444", &view, 1)){
      *with_camera_top = true;
    }
  }

  //---------------------------
}
void GUI_Online::parameter_recorder(){
  //---------------------------

  //Save frame in folder for AI module
  bool* with_save_frame = recordManager->get_with_save_frame();
  ImGui::Checkbox("Save frame", with_save_frame);
  if(*with_save_frame){
    // set save image number
    static bool with_number = true;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::Checkbox("##12323", &with_number)){
      recordManager->select_frame_unlimited(!with_number);
    }
    ImGui::SameLine();

    // If not set number of image
    int* save_frame_max = recordManager->get_save_frame_max();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Number##1", save_frame_max);

    //Path where images are saved
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::Button("...##23")){
      recordManager->select_path_frame();
    }
    ImGui::SameLine();
    string path = recordManager->get_path_frame();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());
  }

  //Save image for interfacing
  bool* with_save_image = recordManager->get_with_save_image();
  ImGui::Checkbox("Save image", with_save_image);
  if(*with_save_image){
    // set save image number
    static bool with_number = true;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::Checkbox("##1232", &with_number)){
      recordManager->select_image_unlimited(!with_number);
    }
    ImGui::SameLine();

    // If not set number of image
    int* save_image_max = recordManager->get_save_image_max();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Number##2", save_image_max);

    //Path where images are saved
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    if(ImGui::Button("...##25")){
      recordManager->select_path_image();
    }
    ImGui::SameLine();
    string path = get_path_abs(recordManager->get_path_image());
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());
  }

  //---------------------------
}

//State function
void GUI_Online::state_time(){
  /*Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = collection->get_sele
  selected_obj->get_frame();
  Frame* frame = collection->get_frame_selected();
  //---------------------------

  float time_operation = onlineManager->get_time_operation();
  ImGui::Text("Operation");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_operation);

  int time_slam = 0;
  if(sceneManager->get_is_list_empty() == false){
    time_slam = (int)frame->time_slam;
  }
  ImGui::Text("SLAM");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", time_slam);

  bool with_save_frame = *recordManager->get_with_save_frame();
  int time_save_frame = 0;
  if(sceneManager->get_is_list_empty() == false){
    time_save_frame = (int)frame->time_save_frame;
  }
  if(with_save_frame){
    ImGui::Text("Save frame");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", time_save_frame);
  }

  bool with_save_image = *recordManager->get_with_save_frame();
  if(with_save_image){
    float time_screenshot = screenshotManager->get_time_screenshot();
    ImGui::Text("Save image");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%d ms", (int)time_screenshot);
  }
*/
  //---------------------------
}
void GUI_Online::state_configuration(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Configuration");
  //---------------------------

  //Choose configuration
  int config_selected = *configManager->get_config();
  if(ImGui::Combo("##007", &config_selected, "Default\0Capture\0AI\0Server\0")){
    configManager->make_preconfig(config_selected);
    node_engine->update();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_Online::state_online(){
  //---------------------------

  bool with_camera_follow = *followManager->get_with_camera_follow();
  ImGui::Text("Online - Camera follow");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_camera_follow ? "ON" : "OFF");

  string color_name = colorManager->get_color_mode_name();
  ImGui::Text("Online - Colorization");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", color_name.c_str());

  bool with_save_frame = *recordManager->get_with_save_frame();
  ImGui::Text("Online - Save frame");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_frame ? "ON" : "OFF");

  bool with_save_image = *recordManager->get_with_save_image();
  ImGui::Text("Online - Save image");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", with_save_image ? "ON" : "OFF");

  //---------------------------
  ImGui::Separator();
}
