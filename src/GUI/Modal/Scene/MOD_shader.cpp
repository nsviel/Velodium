#include "MOD_shader.h"

#include "image/IconsFontAwesome5.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Engine/Shader/Shader.h"
#include "../../../Engine/Shader/Source/Shader_edl.h"
#include "../../../Engine/Shader/Source/Shader_light.h"

#include <fstream>

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_shader::MOD_shader(Node_engine* node_engine){
  //---------------------------

  this->shaderManager = node_engine->get_shaderManager();

  this->item_width = 150;

  //---------------------------
}
MOD_shader::~MOD_shader(){}

//Main function
void MOD_shader::window_shader(){
  bool* open = &modal_tab.show_shader;
  if(*open){
    ImGui::Begin("Shader manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->shader_lighting();
    this->shader_render();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}

//Sub functions
void MOD_shader::shader_lighting(){
  Shader_light* shader_lighting = (Shader_light*)shaderManager->get_shader_src_byName("light");
  //---------------------------

  vec3 color = shader_lighting->get_light_color();
  ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs;
  if(ImGui::ColorEdit3("Color##43", (float*)&color, flags)){
    shader_lighting->set_light_color(color);
    shader_lighting->update_shader();
  }

  //---------------------------
}
void MOD_shader::shader_render(){
  Shader_edl* shader_edl = (Shader_edl*)shaderManager->get_shader_src_byName("render_edl");
  //---------------------------

  /*ImGui::SetNextItemWidth(item_width);
  bool* with_inv = shader_edl->get_with_inv();
  if(ImGui::Checkbox("Color inversion", with_inv)){
    shader_edl->update_shader();
  }*/

  ImGui::SetNextItemWidth(item_width);
  bool* with_edl = shader_edl->get_with_edl();
  if(ImGui::Checkbox("EDL shader", with_edl)){
    shader_edl->update_shader();
  }

  ImGui::SetNextItemWidth(item_width);
  float* edl_radius = shader_edl->get_edl_radius();
  if(ImGui::SliderFloat("EDL radius", edl_radius, 1.0f, 3.0f)){
    shader_edl->update_shader();
  }

  ImGui::SetNextItemWidth(item_width);
  float* edl_strength = shader_edl->get_edl_strength();
  if(ImGui::SliderFloat("EDL strength", edl_strength, 1.0f, 1000.0f)){
    shader_edl->update_shader();
  }

  //---------------------------
}
