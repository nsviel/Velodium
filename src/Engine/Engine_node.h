#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "Scene.h"
#include "Glyphs.h"

#include "Shader/Shader.h"
#include "Configuration/Configuration_node.h"
#include "Configuration/Dimension.h"
#include "Configuration/config_opengl.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Viewport.h"

#include "../common.h"


class Engine_node
{
public:
  //Constructor / Destructor
  Engine_node(Configuration_node* config){
    //---------------------------

    this->node_config = config;
    this->init_objects();

    //---------------------------
  }
  ~Engine_node();

public:
  //Main functions
  void init_objects(){
    //---------------------------

    Dimension* dimManager = node_config->get_dimManager();
    this->shaderManager = new Shader(dimManager);
    this->cameraManager = new Camera(dimManager);
    this->renderManager = new Renderer(dimManager);
    this->viewportManager = new Viewport(dimManager);
    this->sceneManager = new Scene();
    this->glyphManager = new Glyphs();

    //---------------------------
  }
  void reset(){
    //---------------------------
    glyphManager->reset();
    //---------------------------
  }

  inline Configuration_node* get_node_config(){return node_config;}
  
  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Camera* get_cameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline Viewport* get_viewportManager(){return viewportManager;}

private:
  Configuration_node* node_config;

  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  Shader* shaderManager;
  Renderer* renderManager;
  Viewport* viewportManager;
};

#endif
