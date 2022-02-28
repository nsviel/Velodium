#include "Engine_node.h"

#include "Engine.h"

#include "Scene/Configuration.h"
#include "Scene/Scene.h"
#include "Scene/Glyphs.h"

#include "OpenGL/Dimension.h"
#include "OpenGL/Camera/Renderer.h"
#include "OpenGL/Camera/Camera.h"
#include "OpenGL/Camera/Viewport.h"
#include "OpenGL/Shader/Shader.h"

#include "../Load/Load_node.h"
#include "../Operation/Operation_node.h"
#include "../Module/Module_node.h"
#include "../GUI/GUI_node.h"


//Constructor / Destructor
Engine_node::Engine_node(Configuration* config, GLFWwindow* window){
  this->configManager = config;
  //---------------------------

  this->dimManager = new Dimension(window, configManager);
  this->shaderManager = new Shader(dimManager);
  this->cameraManager = new Camera(dimManager);
  this->renderManager = new Renderer(dimManager);
  this->viewportManager = new Viewport(dimManager);
  this->glyphManager = new Glyphs(this);
  this->sceneManager = new Scene(this);

  this->node_load = new Load_node(this);
  this->node_ope = new Operation_node(this);
  this->node_module = new Module_node(this);
  this->node_gui = new GUI_node(this);

  this->engineManager = new Engine(this);

  //---------------------------
}
Engine_node::~Engine_node(){}

void Engine_node::runtime(){}
void Engine_node::reset(){
  //---------------------------

  cameraManager->viewport_reset();
  glyphManager->reset_scene();
  sceneManager->reset_cloud_all();

  //---------------------------
}
void Engine_node::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
