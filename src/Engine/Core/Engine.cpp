#include "Engine.h"

#include "../Node_engine.h"
#include "../Scene/Glyph/Glyphs.h"
#include "../Scene/Glyph/Object.h"
#include "../Scene/Scene.h"
#include "../Scene/Configuration.h"
#include "../Camera/Camera.h"

#include "../../GUI/Node_gui.h"
#include "../../GUI/Control/GUI.h"


//Constructor / Destructor
Engine::Engine(Node_engine* engine){
  this->node_engine = engine;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();
  Node_gui* node_gui = node_engine->get_node_gui();

  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->guiManager = node_gui->get_guiManager();
  this->objectManager = node_engine->get_objectManager();
  this->cameraManager = node_engine->get_cameraManager();

  this->is_visualization = configManager->parse_json_b("window", "visualization");

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
    if(cloud->visibility){
      glPointSize(cloud->point_size);
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);
        this->draw_mesh(subset, cloud->draw_type);
      }
    }
  }

  //---------------------------
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
void Engine::runtime_draw_glyph(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Draw glyph scene
  objectManager->runtime_glyph_scene();

  //Draw glyph subset
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    if(cloud->visibility){
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
void Engine::draw_mesh(Subset* subset, string draw_type){
  //---------------------------

  if(subset->visibility){
    // Bind the glyph VAO
    glBindVertexArray(subset->VAO);
    if(draw_type == "point"){
      glDrawArrays(GL_POINTS, 0, subset->xyz.size());
    }
    else if(draw_type == "line"){
      glDrawArrays(GL_LINES, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle"){
      glDrawArrays(GL_TRIANGLES, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle_strip"){
      glDrawArrays(GL_TRIANGLE_STRIP, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle_fan"){
      glDrawArrays(GL_TRIANGLE_FAN, 0, subset->xyz.size());
    }
    else if(draw_type == "quad"){
      glDrawArrays(GL_QUADS, 0, subset->xyz.size());
    }
    else if(draw_type == "quad_strip"){
      glDrawArrays(GL_QUAD_STRIP, 0, subset->xyz.size());
    }
    else if(draw_type == "polygon"){
      glDrawArrays(GL_POLYGON, 0, subset->xyz.size());
    }
    else{
      glDrawArrays(GL_POINTS, 0, subset->xyz.size());
    }
    glBindVertexArray(0);
  }

  //---------------------------
}
