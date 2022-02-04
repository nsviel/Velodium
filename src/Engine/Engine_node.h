#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "Scene.h"
#include "Glyphs.h"

#include "Shader/Shader.h"
#include "Configuration/Dimension.h"
#include "Configuration/Configuration.h"
#include "Configuration/config_opengl.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Camera.h"

#include "../common.h"


class Operation_node
{
public:
  //Constructor / Destructor
  Operation_node(){
    this->init_objects();
  }
  ~Operation_node();

public:
  //Main functions
  void init_objects(){
    this->dimManager = new Dimension(window);
    this->shaderManager = new Shader(dimManager);
    this->cameraManager = new Camera(dimManager);
    this->renderManager = new Renderer(dimManager);
    this->configManager = new Configuration();
    this->sceneManager = new Scene();
    this->glyphManager = new Glyphs();
  }

  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Camera* get_CameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline Configuration* get_configManager(){return configManager;}

private:
  Dimension* dimManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  Shader* shaderManager;
  Renderer* renderManager;
  Configuration* configManager;
};

#endif
