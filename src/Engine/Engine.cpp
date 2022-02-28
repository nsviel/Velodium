#include "Engine.h"

#include "Engine_node.h"
#include "Scene/Glyphs.h"
#include "Scene/Scene.h"

#include "../Module/Module_node.h"
#include "../GUI/GUI_node.h"
#include "../GUI/GUI.h"


//Constructor / Destructor
Engine::Engine(Engine_node* engine){
  this->with_window = true;
  this->node_engine = engine;
  //---------------------------

  this->node_module = node_engine->get_node_module();
  this->node_gui = node_engine->get_node_gui();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->guiManager = node_gui->get_guiManager();

  //---------------------------
}
Engine::~Engine(){
  //---------------------------

  delete sceneManager;
  delete glyphManager;

  //---------------------------
}

//Program functions
void Engine::loop_scene(){
  //---------------------------

  //Draw glyph stuff
  if(with_window){
    glyphManager->runtime_glyph_scene();

    //Draw clouds
    this->draw_clouds();

    //Runtime functions
    node_gui->runtime();
  }

  //Runtime functions
  node_engine->runtime();
  node_module->runtime();

  //---------------------------
}
void Engine::loop_gui(){
  //---------------------------

  if(with_window){
    guiManager->Gui_loop();
  }

  //---------------------------
}
void Engine::draw_clouds(){
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

          glyphManager->runtime_glyph_subset(subset);
        }

        //Display for current subset
        if(subset->ID == subset_selected->ID && subset->obstacle_pr.oobb.size() != 0){
          int oID = sceneManager->get_subset_oID(cloud, subset);
          if(oID > 0){
            Subset* subset_m1 = *next(cloud->subset.begin(), j - 1);
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
