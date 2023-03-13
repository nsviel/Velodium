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
  list<Collection*>* list_collection = sceneManager->get_list_collection();
  //---------------------------

  //Draw glyph scene
  objectManager->runtime_glyph_scene();

  //Draw glyph cloud
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    if(collection->is_visible){
      //All cloud
      objectManager->runtime_glyph_subset_all(collection);

      //Selected susbet
      Cloud* subset_sele = (Cloud*)collection->get_obj_byID(collection->ID_obj_selected);
      objectManager->runtime_glyph_subset_selected(subset_sele);

      //OOBB
      Cloud* subset_pred = (Cloud*)collection->get_obj_byID(collection->ID_obj_selected - 2);
      objectManager->runtime_glyph_pred(subset_pred);
    }

  }

  //---------------------------
}
void Engine::arcball_cam_lookat(){
  Collection* collection = data->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  //Pour arcball camera view, center cam F to cloud com
  if(cloud != nullptr){
    vec3* cam_COM = cameraManager->get_cam_COM();
    *cam_COM = cloud->COM;
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
  list<Collection*>* list_collection = sceneManager->get_list_collection();
  //---------------------------

  //By collection
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    //By cloud
    if(collection->is_visible){
      for(int j=0; j<collection->list_obj.size(); j++){
        Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), j);

        if(cloud->is_visible && cloud->has_texture == false){
          gpuManager->draw_object(cloud);
        }

      }
    }
  }

  //---------------------------
}
void Engine::draw_textured_cloud(){
  list<Collection*>* list_collection = sceneManager->get_list_collection();
  bool with_texture = *texManager->get_with_texture();
  //---------------------------

  //By collection
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    //By cloud
    if(collection->is_visible){
      for(int j=0; j<collection->list_obj.size(); j++){
        Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), j);

        if(cloud->is_visible && cloud->has_texture && with_texture){
          gpuManager->bind_texture(cloud->tex_ID);
          gpuManager->draw_object(cloud);
        }

      }
    }
  }

  //---------------------------
}
