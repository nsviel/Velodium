#include "Engine.h"

#include "Engine_node.h"
#include "Scene/Glyphs.h"
#include "Scene/Scene.h"
#include "Scene/Configuration.h"

#include "../GUI/GUI_node.h"
#include "../GUI/GUI.h"


//Constructor / Destructor
Engine::Engine(Engine_node* engine){
  this->node_engine = engine;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();

  this->node_gui = node_engine->get_node_gui();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->guiManager = node_gui->get_guiManager();

  this->is_visualization = configManager->parse_json_b("window", "visualization");

  //---------------------------
}
Engine::~Engine(){}

//Program functions
void Engine::loop_scene(){
  //---------------------------

  //Draw glyph stuff
  if(is_visualization){
    glyphManager->runtime_glyph_scene();

    //Draw clouds
    this->runtime_draw_clouds();

    //Runtime functions
    node_gui->runtime();
  }

  //Runtime
  node_engine->runtime();

  //---------------------------
}
void Engine::loop_gui(){
  //---------------------------

  guiManager->Gui_loop();

  //---------------------------
}
void Engine::runtime_draw_clouds(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  Subset* subset_selected = sceneManager->get_subset_selected();
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
          glDrawArrays(GL_POINTS, 0, subset->xyz.size());
          glBindVertexArray(0);

          //Subset glyph stuff
          glyphManager->runtime_glyph_subset(subset);
          if(subset->ID == subset_selected->ID && j > 2){say("okkkkkkkkay");
            Subset* subset_m1 = *next(cloud->subset.begin(), j-1);
            glyphManager->runtime_glyph_pred(subset_m1);
          }

        }
      }
    }

  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
