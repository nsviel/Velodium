#include "Node_engine.h"

#include "Core/Engine.h"
#include "Core/Configuration.h"
#include "Core/Dimension.h"

#include "OpenGL/CoreGLengine.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Light.h"
#include "Shader/Shader.h"

#include "Rendering/Renderer.h"
#include "GPU/GPU_screenshot.h"
#include "GPU/GPU_fbo.h"
#include "GPU/GPU_render.h"

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

  this->dimManager = new Dimension();
  this->configManager = new Configuration();
  this->node_scene = new Node_scene(this);
  this->shaderManager = new Shader(this);
  this->viewportManager = new Viewport(this);
  this->cameraManager = new Camera(this);
  this->followManager = new Followup(this);
  this->gpu_screenshot = new GPU_screenshot(this);
  this->gpu_fbo = new GPU_fbo(this);
  this->gpu_render = new GPU_render();
  this->texManager = new Texture();
  this->lightManager = new Light(this);

  this->node_load = new Node_load(this);
  this->node_ope = new Node_operation(this);
  this->node_interface = new Node_interface(this);
  this->node_module = new Node_module(this);

  this->engineManager = new Engine(this);
  this->renderManager = new Renderer(this);

  this->node_gui = new Node_gui(this);

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
  node_scene->update();

  followManager->update_configuration();

  //---------------------------
}
void Node_engine::runtime(){
  //---------------------------

  node_module->runtime();
  node_ope->runtime();
  node_interface->runtime();

  //---------------------------
}
void Node_engine::reset(){
  //---------------------------

  node_scene->reset();
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
