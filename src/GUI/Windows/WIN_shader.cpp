#include "WIN_shader.h"

#include "../../Engine/Shader/Shader.h"
#include "../../Engine/Shader/ShaderObject.h"
#include "../../Engine/Shader/PP_edl.h"

#include "Window_tab.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_shader::WIN_shader(Shader* shader){
  //---------------------------

  this->shaderManager = shader;
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

    ImGui::SetNextItemWidth(item_width);
    bool* with_edl = edlManager->get_with_edl();
    if(ImGui::Checkbox("EDL shader", with_edl)){
      ShaderObject* shader_screen = shaderManager->get_shader_screen();
      edlManager->setup_edl(shader_screen->get_program_ID());
    }

    ImGui::SetNextItemWidth(item_width);
    float* edl_strength = edlManager->get_edl_strength();
    if(ImGui::SliderFloat("EDL strength", edl_strength, 1.0f, 1000.0f)){
      ShaderObject* shader_screen = shaderManager->get_shader_screen();
      edlManager->setup_edl(shader_screen->get_program_ID());
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}
