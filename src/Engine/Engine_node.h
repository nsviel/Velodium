#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "Scene/Configuration.h"
#include "Scene/Scene.h"
#include "Scene/Glyphs.h"

#include "OpenGL/Dimension.h"
#include "OpenGL/Camera/Renderer.h"
#include "OpenGL/Camera/Camera.h"
#include "OpenGL/Camera/Viewport.h"
#include "OpenGL/Shader/Shader.h"

#include "../common.h"


class Engine_node
{
public:
  //Constructor / Destructor
  Engine_node(Configuration* config, GLFWwindow* window){
    //---------------------------

    this->configManager = config;

    //---------------------------
    this->init_objects(window);
  }
  ~Engine_node();

public:
  //Main functions
  void init_objects(GLFWwindow* window){
    //---------------------------

    this->dimManager = new Dimension(window, configManager);
    this->shaderManager = new Shader(dimManager);
    this->cameraManager = new Camera(dimManager);
    this->renderManager = new Renderer(dimManager);
    this->viewportManager = new Viewport(dimManager);
    this->sceneManager = new Scene();
    this->glyphManager = new Glyphs();

    //---------------------------
  }
  void runtime(){}
  void reset(){
    //---------------------------

    glyphManager->reset();
    sceneManager->reset_cloud_all();

    //---------------------------
  }

  inline Configuration* get_configManager(){return configManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Camera* get_cameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline Viewport* get_viewportManager(){return viewportManager;}

private:
  Configuration* configManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  Shader* shaderManager;
  Renderer* renderManager;
  Viewport* viewportManager;
  Dimension* dimManager;
};

#endif
