#include "Engine.h"

#include "Scene.h"
#include "Glyphs.h"

#include "Data/Database.h"
#include "Shader/Shader.h"
#include "Configuration/Dimension.h"
#include "Configuration/Configuration.h"
#include "OpenGL/CoreGLengine.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Camera.h"

#include "../Operation/Functions/Heatmap.h"
#include "../Operation/Transformation/Filter.h"
#include "../Module/SLAM/CT_ICP.h"

Database database;


//Constructor / Destructor
Engine::Engine(Dimension* dim, Camera* control, Shader* shader, Renderer* render){
  this->dimManager = dim;
  this->shaderManager = shader;
  this->cameraManager = control;
  this->renderManager = render;
  //---------------------------

  this->init_database();

  this->cticpManager = new CT_ICP();
  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();
  this->heatmapManager = new Heatmap();
  this->filterManager = new Filter();

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
void Engine::loop(){
  Configuration configManager;
  //---------------------------

  //Draw glyph stuff
  glyphManager->drawing();

  //Draw clouds
  this->draw_clouds();

  //---------------------------
}
void Engine::Exit(){
  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);
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
  glyphManager->reset();
  cticpManager->reset();
  sceneManager->update_cloud_glyphs(cloud);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Subfunctions
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
        Subset* subset = &next(cloud->subset.begin(), j);

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
