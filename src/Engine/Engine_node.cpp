#include "Engine_node.h"

#include "Engine.h"

#include "Scene/Configuration.h"
#include "Scene/Scene.h"
#include "Scene/Glyphs.h"
#include "Scene/Object.h"

#include "OpenGL/Dimension.h"
#include "OpenGL/CoreGLengine.h"
#include "OpenGL/Camera/Renderer.h"
#include "OpenGL/Camera/Camera.h"
#include "OpenGL/Camera/Followup.h"
#include "OpenGL/Camera/Viewport.h"
#include "OpenGL/Shader/Shader.h"

#include "../Load/Load_node.h"
#include "../Operation/Node_operation.h"
#include "../Module/Module_node.h"
#include "../GUI/GUI_node.h"


//Constructor / Destructor
Engine_node::Engine_node(CoreGLengine* ogl){
  this->glManager = ogl;
  //---------------------------

  this->configManager = ogl->get_configManager();
  this->dimManager = new Dimension(ogl->get_window(), configManager);
  this->shaderManager = new Shader(dimManager);
  this->cameraManager = new Camera(dimManager);
  this->followManager = new Followup(this);
  this->renderManager = new Renderer(dimManager);
  this->viewportManager = new Viewport(dimManager);
  this->glyphManager = new Glyphs(this);
  this->objectManager = new Object(this);
  this->sceneManager = new Scene(this);

  this->node_load = new Load_node(this);
  this->node_ope = new Node_operation(this);
  this->node_module = new Module_node(this);
  this->node_gui = new GUI_node(this);

  this->engineManager = new Engine(this);

  //---------------------------
}
Engine_node::~Engine_node(){}

void Engine_node::update(){
  //---------------------------

  node_module->update();
  node_ope->update();
  node_load->update();

  objectManager->update_configuration();
  followManager->update_configuration();

  //---------------------------
}
void Engine_node::runtime(){
  //---------------------------

  node_module->runtime();

  //---------------------------
}
void Engine_node::reset(){
  //---------------------------

  cameraManager->viewport_reset();
  objectManager->reset_scene_object();
  sceneManager->reset_cloud_all();
  followManager->camera_reset();

  //---------------------------
}
void Engine_node::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
