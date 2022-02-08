#include "Engine.h"

#include "Scene.h"
#include "Glyphs.h"
#include "Data/Database.h"

#include "Engine_node.h"
#include "Configuration/Configuration_node.h"
#include "../Operation/Operation_node.h"
#include "../Module/Module_node.h"
#include "../GUI/GUI_node.h"
#include "../GUI/GUI.h"

Database database;


//Constructor / Destructor
Engine::Engine(Engine_node* engine){
  //---------------------------

  this->node_engine = engine;
  this->node_config = node_engine->get_node_config();

  this->init_database();

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();

  this->node_ope = new Operation_node(node_engine);
  this->node_module = new Module_node(node_engine, node_ope);
  this->node_gui = new GUI_node(node_engine, node_ope, node_module, node_config);

  glyphManager->init();

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
  glyphManager->drawing();

  //Draw clouds
  this->draw_clouds();

  //Module stuff
  this->loop_runtime();

  //---------------------------
}
void Engine::loop_runtime(){
  //---------------------------

  node_gui->runtime();
  node_engine->runtime();
  node_module->runtime();

  //---------------------------
}
void Engine::loop_gui(){
  GUI* guiManager = node_gui->get_guiManager();
  guiManager->Gui_loop();
}
void Engine::draw_clouds(){
  list<Cloud*>* list_cloud = database.list_cloud;
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    glPointSize(cloud->point_size);

    //By subset
    if(cloud->visibility){
      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);

        //Display vertices
        if(subset->visibility){
          sceneManager->update_subset_location(subset);
          glBindVertexArray(subset->VAO);
          glDrawArrays(GL_POINTS, 0, subset->xyz.size());
          glBindVertexArray(0);

          glyphManager->drawing(subset);
        }

      }
    }

  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
