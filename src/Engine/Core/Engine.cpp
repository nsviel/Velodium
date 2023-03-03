#include "Engine.h"

#include "../Node_engine.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/GPU_transfert.h"
#include "../Core/Configuration.h"
#include "../Camera/Camera.h"

#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Glyph/Glyphs.h"
#include "../../Scene/Glyph/Object.h"
#include "../../GUI/Node_gui.h"
#include "../../GUI/Control/GUI.h"


//Constructor / Destructor
Engine::Engine(Node_engine* engine){
  this->node_engine = engine;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();
  Node_gui* node_gui = node_engine->get_node_gui();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->gpuManager = new GPU_transfert();
  this->sceneManager = node_scene->get_sceneManager();
  this->glyphManager = node_scene->get_glyphManager();
  this->guiManager = node_gui->get_guiManager();
  this->objectManager = node_scene->get_objectManager();

  this->cameraManager = node_engine->get_cameraManager();
  this->texManager = node_engine->get_texManager();

  //---------------------------
}
Engine::~Engine(){}

//Program functions
void Engine::runtime_scene(){
  //---------------------------

  //Runtime cloud
  this->runtime_draw_cloud();

  //Runtime glyph
  this->runtime_draw_glyph();

  //Runtime camera
  this->runtime_camera();

  //---------------------------
}
void Engine::runtime_draw_cloud(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //By subset
    if(cloud->is_visible){
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);
        this->draw_cloud(subset);
      }
    }
  }

  //---------------------------
}
void Engine::runtime_draw_glyph(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Draw glyph scene
  objectManager->runtime_glyph_scene();

  //Draw glyph subset
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    if(cloud->is_visible){
      //All subset
      objectManager->runtime_glyph_subset_all(cloud);

      //Selected susbet
      Subset* subset_sele = sceneManager->get_subset_byID(cloud, cloud->ID_selected);
      objectManager->runtime_glyph_subset_selected(subset_sele);

      //OOBB
      Subset* subset_pred = sceneManager->get_subset_byID(cloud, cloud->ID_selected - 2);
      objectManager->runtime_glyph_pred(subset_pred);
    }

  }

  //---------------------------
}
void Engine::runtime_camera(){
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  if(subset != nullptr){
    vec3* cam_COM = cameraManager->get_cam_COM();
    *cam_COM = subset->COM;
  }

  //---------------------------
}

//Subfunction
void Engine::draw_cloud(Subset* subset){
  bool with_texture = *texManager->get_with_texture();
  //---------------------------

  if(subset->is_visible){
    // If any, activate attached texture
    if(with_texture && subset->has_texture){
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, subset->texture_ID);
    }

    if(subset->is_visible){
      gpuManager->draw_object(subset);
    }

    //Desactivate texture
    if(with_texture && subset->has_texture){
      glBindTexture(GL_TEXTURE_2D, 2);
    }
  }

  //---------------------------
}
