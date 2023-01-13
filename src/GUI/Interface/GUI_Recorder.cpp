#include "GUI_Recorder.h"

#include "../Node_gui.h"

#include "../../Interface/Node_interface.h"
#include "../../Interface/IO/Recorder.h"
#include "../../Interface/File/Path.h"


//Constructor / Destructor
GUI_Recorder::GUI_Recorder(Node_gui* node_gui){
  //---------------------------

  Node_interface* node_interface = node_gui->get_node_interface();

  this->recordManager = node_interface->get_recordManager();

  this->item_width = 100;

  //---------------------------
}
GUI_Recorder::~GUI_Recorder(){}

//Main function
void GUI_Recorder::design_recorder(){
  //---------------------------

  this->recorder_frame();
  this->recorder_screenshot();

  //---------------------------
}

//Specific functions
void GUI_Recorder::recorder_frame(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Frame");
  //---------------------------

  //Save frame in folder for AI module
  bool* with_save_frame = recordManager->get_with_save_frame();
  ImGui::Checkbox("Activated##1", with_save_frame);
  ImGui::SameLine();

  //Save frame without slam
  bool* with_save_frame_raw = recordManager->get_with_save_frame_raw();
  ImGui::Checkbox("Raw", with_save_frame_raw);

  // set save frame number
  static bool with_number = true;
  if(ImGui::Checkbox("##12323", &with_number)){
    recordManager->select_frame_unlimited(!with_number);
  }
  ImGui::SameLine();

  // If not set number of frame
  int* save_frame_max = recordManager->get_save_frame_max();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Number##1", save_frame_max);

  // Accumulated frames
  int* save_frame_accu = recordManager->get_save_frame_accu();
  ImGui::SetNextItemWidth(120);
  ImGui::InputInt("Accumulated##1", save_frame_accu);

  //Path where frame are saved
  if(ImGui::Button("...##23")){
    recordManager->select_path_frame();
  }
  ImGui::SameLine();
  string path = get_path_abs(recordManager->get_path_frame());
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());


  //---------------------------
  ImGui::Separator();
}
void GUI_Recorder::recorder_screenshot(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Screenshot");
  //---------------------------

  //Save image for interfacing
  bool* with_save_image = recordManager->get_with_save_image();
  ImGui::Checkbox("Activated##2", with_save_image);

  // set save image number
  static bool with_number = true;
  if(ImGui::Checkbox("##1232", &with_number)){
    recordManager->select_image_unlimited(!with_number);
  }
  ImGui::SameLine();

  // If not set number of image
  int* save_image_max = recordManager->get_save_image_max();
  ImGui::SetNextItemWidth(100);
  ImGui::InputInt("Number##2", save_image_max);

  //Path where images are saved
  if(ImGui::Button("...##25")){
    recordManager->select_path_image();
  }
  ImGui::SameLine();
  string path = get_path_abs(recordManager->get_path_image());
  ImGui::TextColored(ImVec4(0.0f,1.0f,1.0f,1.0f), "%s", path.c_str());

  //---------------------------
  ImGui::Separator();
}
