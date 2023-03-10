#include "MOD_texture.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Engine/OpenGL/Texture.h"
#include "../../../Scene/Data/Scene.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_texture::MOD_texture(Node_engine* node_engine){
  //---------------------------

  Node_scene* node_scene = node_engine->get_node_scene();

  this->texManager = node_engine->get_texManager();
  this->sceneManager = node_scene->get_sceneManager();

  this->item_width = 150;

  //---------------------------
}
MOD_texture::~MOD_texture(){}

//Main function
void MOD_texture::window_texture(){
  bool* open = &modal_tab.show_texture;
  if(*open){
    ImGui::Begin("Texture manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->texture_operation();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}
void MOD_texture::texture_operation(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Apply texture on selected cloud
  if(ImGui::Button("Apply texture", ImVec2(item_width, 0))){

  }

  //---------------------------
}
