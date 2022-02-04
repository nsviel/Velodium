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
    this->dimManager = dim;
    this->shaderManager = shader;
    this->cameraManager = control;
    this->renderManager = render;
    this->configManager = config;
    this->sceneManager = new Scene();
    this->glyphManager = new Glyphs();
  }

  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}

private:
  Heatmap* heatmapManager;
  Filter* filterManager;
};

#endif
