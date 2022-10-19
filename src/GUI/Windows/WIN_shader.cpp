#include "WIN_shader.h"

#include "IconsFontAwesome5.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Shader/Shader.h"
#include "../../Engine/OpenGL/Shader/ShaderObject.h"
#include "../../Engine/OpenGL/Shader/PP_edl.h"

#include <fstream>

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_shader::WIN_shader(Node_engine* node_engine){
  //---------------------------

  this->shaderManager = node_engine->get_shaderManager();
  this->edlManager = shaderManager->get_edlManager();

  this->item_width = 150;

  //---------------------------
}
WIN_shader::~WIN_shader(){}

//Main function
void WIN_shader::window_shader(){
  bool* open = &window_tab.show_shader;
  if(*open){
    ImGui::Begin("Shader manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->parameter();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}

//Sub functions
void WIN_shader::parameter(){
  //---------------------------

  ImGui::SetNextItemWidth(item_width);
  bool* with_edl = edlManager->get_with_edl();
  if(ImGui::Checkbox("EDL shader", with_edl)){
    ShaderObject* shader_screen = shaderManager->get_shader_screen();
    edlManager->setup_edl(shader_screen->get_program_ID());
  }

  ImGui::SetNextItemWidth(item_width);
  float* edl_radius = edlManager->get_edl_radius();
  if(ImGui::SliderFloat("EDL radius", edl_radius, 1.0f, 3.0f)){
    ShaderObject* shader_screen = shaderManager->get_shader_screen();
    edlManager->setup_edl(shader_screen->get_program_ID());
  }

  ImGui::SetNextItemWidth(item_width);
  float* edl_strength = edlManager->get_edl_strength();
  if(ImGui::SliderFloat("EDL strength", edl_strength, 1.0f, 5000.0f)){
    ShaderObject* shader_screen = shaderManager->get_shader_screen();
    edlManager->setup_edl(shader_screen->get_program_ID());
  }

  //---------------------------
}
