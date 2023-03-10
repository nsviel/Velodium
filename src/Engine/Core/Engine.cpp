#include "Engine.h"

#include "../Node_engine.h"
#include "../OpenGL/Texture.h"
#include "../GPU/GPU_data.h"
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

  this->gpuManager = new GPU_data();
  this->sceneManager = node_scene->get_sceneManager();
  this->glyphManager = node_scene->get_glyphManager();
  this->guiManager = node_gui->get_guiManager();
  this->objectManager = node_scene->get_objectManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->texManager = node_engine->get_texManager();
  this->data = Data::get_instance();

  //---------------------------
}
Engine::~Engine(){}

//Program functions
void Engine::draw_untextured_glyph(){
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
      Subset* subset_sele = cloud->get_subset_byID(cloud->ID_selected);
      objectManager->runtime_glyph_subset_selected(subset_sele);

      //OOBB
      Subset* subset_pred = cloud->get_subset_byID(cloud->ID_selected - 2);
      objectManager->runtime_glyph_pred(subset_pred);
    }

  }

  //---------------------------
}
void Engine::arcball_cam_lookat(){
  Cloud* cloud = data->get_cloud_selected();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Pour arcball camera view, center cam F to subset com
  if(subset != nullptr){
    vec3* cam_COM = cameraManager->get_cam_COM();
    *cam_COM = subset->COM;
  }

  //---------------------------
}

//Cloud drawing function
void Engine::draw_light(){
  std::list<Light_*>* list_light = data->get_list_light();
  //---------------------------

  for(int i=0; i<list_light->size(); i++){
    Light_* light = *next(list_light->begin(),i);
    gpuManager->draw_object(light);
  }

  //---------------------------
}
void Engine::draw_untextured_cloud(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //By subset
    if(cloud->is_visible){
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);

        if(subset->is_visible && subset->has_texture == false){
          gpuManager->draw_object(subset);
        }

      }
    }
  }

  //---------------------------
}
void Engine::draw_textured_cloud(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  bool with_texture = *texManager->get_with_texture();
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //By subset
    if(cloud->is_visible){
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);

        if(subset->is_visible && subset->has_texture && with_texture){
          gpuManager->bind_texture(subset->tex_ID);
          gpuManager->draw_object(subset);
        }

      }
    }
  }

  //---------------------------
}
