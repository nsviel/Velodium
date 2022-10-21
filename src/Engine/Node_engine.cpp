#include "Node_engine.h"

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

#include "../Load/Node_load.h"
#include "../Operation/Node_operation.h"
#include "../Module/Node_module.h"
#include "../Interface/Node_interface.h"
#include "../GUI/Node_gui.h"


//Constructor / Destructor
Node_engine::Node_engine(CoreGLengine* ogl){
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

  this->node_load = new Node_load(this);
  this->node_ope = new Node_operation(this);
  this->node_interface = new Node_interface(this);
  this->node_module = new Node_module(this);
  this->node_gui = new Node_gui(this);

  this->engineManager = new Engine(this);

  //---------------------------
}
Node_engine::~Node_engine(){}

void Node_engine::update(){
  //---------------------------

  node_module->update();
  node_ope->update();
  node_load->update();

  objectManager->update_configuration();
  followManager->update_configuration();

  //---------------------------
}
void Node_engine::runtime(){
  //---------------------------

  engineManager->runtime_scene();
  node_module->runtime();
  node_ope->runtime();

  //---------------------------
}
void Node_engine::reset(){
  //---------------------------

  cameraManager->viewport_reset();
  objectManager->reset_scene_object();
  sceneManager->reset_cloud_all();
  followManager->camera_reset();

  //---------------------------
}
void Node_engine::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
