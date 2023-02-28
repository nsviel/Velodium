#include "Node_engine.h"

#include "Core/Engine.h"
#include "Core/Dimension.h"
#include "Core/Configuration.h"

#include "OpenGL/Drawer.h"
#include "OpenGL/CoreGLengine.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Light.h"
#include "Shader/Shader.h"

#include "Camera/Camera.h"
#include "Camera/Followup.h"
#include "Camera/Viewport.h"

#include "../Load/Node_load.h"
#include "../Operation/Node_operation.h"
#include "../Module/Node_module.h"
#include "../Interface/Node_interface.h"
#include "../Scene/Node_scene.h"
#include "../GUI/Node_gui.h"


//Constructor / Destructor
Node_engine::Node_engine(CoreGLengine* ogl){
  this->glManager = ogl;
  //---------------------------

  this->configManager = ogl->get_configManager();
  this->node_scene = new Node_scene(this);
  
  this->dimManager = new Dimension(ogl->get_window(), configManager);
  this->shaderManager = new Shader(this);
  this->viewportManager = new Viewport(dimManager);
  this->cameraManager = new Camera(this);
  this->followManager = new Followup(this);
  this->renderManager = new Renderer(dimManager);
  this->texManager = new Texture();
  this->drawManager = new Drawer(this);

  this->node_load = new Node_load(this);
  this->node_ope = new Node_operation(this);
  this->node_interface = new Node_interface(this);
  this->node_module = new Node_module(this);
  this->node_gui = new Node_gui(this);

  this->lightManager = new Light(this);
  this->engineManager = new Engine(this);

  lightManager->init();

  //---------------------------
}
Node_engine::~Node_engine(){
  //---------------------------

  delete renderManager;

  //---------------------------
}

void Node_engine::update(){
  //---------------------------

  node_module->update();
  node_ope->update();
  node_load->update();
  node_interface->update();

  followManager->update_configuration();

  //---------------------------
}
void Node_engine::runtime(){
  //---------------------------

  engineManager->runtime_scene();
  node_module->runtime();
  node_ope->runtime();
  node_interface->runtime();

  //---------------------------
}
void Node_engine::reset(){
  //---------------------------

  node_ope->reset();
  viewportManager->viewport_reset();
  followManager->camera_reset();

  //---------------------------
}
void Node_engine::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
