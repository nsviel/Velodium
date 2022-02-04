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
  Engine_node(){
    //---------------------------
    this->init_configuration();
    //---------------------------
  }
  ~Engine_node();

public:
  //Main functions
  void init_configuration(){
    //---------------------------
    this->node_configManager = new Configuration_node();
    //---------------------------
  }
  void init_objects(GLFWwindow* window){
    //---------------------------
    this->dimManager = new Dimension(window);
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

  inline Configuration_node* get_node_configManager(){return node_configManager;}
  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Camera* get_cameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline Viewport* get_viewportManager(){return viewportManager;}

private:
  Configuration_node* node_configManager;
  Dimension* dimManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  Shader* shaderManager;
  Renderer* renderManager;
  Viewport* viewportManager;
};

#endif
