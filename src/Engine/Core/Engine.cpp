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


//Constructor / Destructor
Engine::Engine(Node_engine* engine){
  this->node_engine = engine;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->gpuManager = new GPU_data();
  this->sceneManager = node_scene->get_sceneManager();
  this->glyphManager = node_scene->get_glyphManager();
  this->objectManager = node_scene->get_objectManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->texManager = node_engine->get_texManager();
  this->data = Data::get_instance();

  //---------------------------
}
Engine::~Engine(){}

//Program functions
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
  Collection* col_light = data->get_collection_byName("glyph", "light");
  //---------------------------

  for(int i=0; i<col_light->list_obj.size(); i++){
    Light_* light = (Light_*)*next(col_light->list_obj.begin(),i);
    gpuManager->draw_object(light);
  }

  //---------------------------
}
void Engine::draw_untextured_cloud(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
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
void Engine::draw_untextured_glyph(){
  //---------------------------

  objectManager->draw_glyph_scene();
  objectManager->draw_glyph_object();

  //---------------------------
}
void Engine::draw_textured_cloud(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
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
