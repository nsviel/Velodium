#include "Engine.h"

#include "Scene.h"
#include "Glyphs.h"
#include "Data/Database.h"

#include "Engine_node.h"
#include "Configuration/Configuration_node.h"
#include "../Operation/Operation_node.h"
#include "../Module/Module_node.h"

Database database;


//Constructor / Destructor
Engine::Engine(Engine_node* engine){
  //---------------------------

  this->node_engineManager = engine;
  this->node_configManager = node_engineManager->get_node_configManager();

  this->init_database();

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();

  this->node_opeManager = new Operation_node();
  this->node_moduleManager = new Module_node(node_engineManager, node_opeManager);

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
  this->runtime();

  //---------------------------
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

//Subfunctions
void Engine::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
void Engine::reset(){
  list<Cloud*>* list_Cloud = sceneManager->get_list_cloud();
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_Cloud->size(); i++){
    Cloud* cloud = *next(list_Cloud->begin(),i);
    sceneManager->update_cloud_reset(cloud);
  }

  //Reset all functions
  node_engineManager->reset();
  node_moduleManager->reset();
  sceneManager->update_cloud_glyphs(cloud);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}
void Engine::runtime(){
  //---------------------------

  node_moduleManager->runtime();

  //---------------------------
}
