#include "Engine.h"

#include "Data/Database.h"
#include "Engine_node.h"
#include "Scene/Glyphs.h"
#include "Scene/Scene.h"

#include "../Operation/Operation_node.h"
#include "../Module/Module_node.h"
#include "../GUI/GUI_node.h"
#include "../GUI/GUI.h"

Database database;


//Constructor / Destructor
Engine::Engine(Engine_node* engine, bool window_ON){
  //---------------------------

  this->with_window = window_ON;
  this->node_engine = engine;

  this->init_database();

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();

  this->node_ope = new Operation_node(node_engine);
  this->node_module = new Module_node(node_engine, node_ope);
  this->node_gui = new GUI_node(node_engine, node_ope, node_module);

  if(with_window){
    glyphManager->init();
  }

  //---------------------------
}
Engine::~Engine(){
  //---------------------------

  delete sceneManager;
  delete glyphManager;

  //---------------------------
}

//Program functions
void Engine::init_database(){
  //---------------------------

  database.list_cloud = new list<Cloud*>;
  database.list_glyph = new list<Glyph*>;
  database.ID_cloud = 0;
  database.ID_glyph = 0;

  //---------------------------
}
void Engine::loop_scene(){
  //---------------------------

  //Draw glyph stuff
  if(with_window){
    glyphManager->drawing();

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
    GUI* guiManager = node_gui->get_guiManager();
    guiManager->Gui_loop();
  }

  //---------------------------
}
void Engine::draw_clouds(){
  list<Cloud*>* list_cloud = database.list_cloud;
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
          sceneManager->update_subset_location(subset);
          glBindVertexArray(subset->VAO);
          glDrawArrays(GL_POINTS, 0, subset->xyz.size());
          glBindVertexArray(0);

          glyphManager->drawing(subset);
        }

        //Display for current subset
        if(subset->ID == subset_selected->ID){
          glyphManager->drawing_prediction(subset);
        }
      }
    }

  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
