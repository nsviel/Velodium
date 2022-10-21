#include "Engine.h"

#include "Node_engine.h"
#include "Scene/Glyphs.h"
#include "Scene/Object.h"
#include "Scene/Scene.h"
#include "Scene/Configuration.h"

#include "../GUI/Node_gui.h"
#include "../GUI/Control/GUI.h"


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

  //---------------------------
}
void Engine::runtime_draw_cloud(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    glPointSize(cloud->point_size);

    //By subset
    if(cloud->visibility){
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);

        //Display for all visible subsets
        if(subset->visibility){
          glBindVertexArray(subset->VAO);
          glDrawArrays(GL_POINTS, 0, subset->xyz.size()); // Error here during capture via pywardium
        }
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
