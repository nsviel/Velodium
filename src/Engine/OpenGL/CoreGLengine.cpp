#include "CoreGLengine.h"

#include "Camera.h"
#include "Viewport.h"
#include "Renderer.h"
#include "Dimension.h"

#include "../Engine.h"
#include "../Engine_node.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderObject.h"
#include "../Configuration/config_opengl.h"
#include "../Configuration/Configuration_node.h"

#include "../../GUI/GUI_node.h"
#include "../../GUI/GUI.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES


//Constructor / Destructor
CoreGLengine::CoreGLengine(){
  //---------------------------


  //---------------------------
}
CoreGLengine::~CoreGLengine(){
  //---------------------------

  glfwDestroyWindow(window);
  glfwTerminate();

  //---------------------------
}

//Engine Initialization
bool CoreGLengine::init(){
  //---------------------------

  this->init_OGL();
  this->init_object();
  this->init_rendering();

  //---------------------------
  console.AddLog("sucess" ,"Program initialized...");
  return true;
}
bool CoreGLengine::init_OGL(){
  //---------------------------

  this->node_config = new Configuration_node();
  config_opengl* conf_glManager = node_config->get_conf_glManager();

  //Parametrization
  int resolution_width = conf_glManager->parse_json_i("window", "resolution_width");
  int resolution_height = conf_glManager->parse_json_i("window", "resolution_height");
  bool forceVersion = conf_glManager->parse_json_b("opengl", "forceVersion");
  bool coreGL_verbose = conf_glManager->parse_json_b("opengl", "verbose_coreGL");
  string win_title = conf_glManager->parse_json_s("window", "title");
  this->waitForEvent = conf_glManager->parse_json_b("opengl", "waitForEvent");

  //GLFW
  glfwInit();
  if(forceVersion){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  }
  glfwWindowHint(GLFW_SAMPLES, 16);
  glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  window = glfwCreateWindow(resolution_width, resolution_height, "window", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }else if(coreGL_verbose){;
    std::cout << "GLFW window created" << std::endl;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetWindowTitle(window, win_title.c_str());

  //Enable OpenGL alpha channel for RGB
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //GLEW
  glewInit();
  if(coreGL_verbose){
    std::cout << "GLEW initiated" << std::endl;
  }

  //---------------------------
  return true;
}
bool CoreGLengine::init_object(){
  //---------------------------

  this->node_engine = new Engine_node(node_config, window);
  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->renderManager = node_engine->get_renderManager();
  this->engineManager = new Engine(node_engine);

  //---------------------------
  return true;
}
void CoreGLengine::init_rendering(){
  //---------------------------


  renderManager->init_rendering_fbo_1();
  renderManager->init_rendering_fbo_2();
  renderManager->init_rendering_quad();
  shaderManager->init();

  //---------------------------
}

void CoreGLengine::loop(){
  //---------------------------

  do{
    //First pass
    //---------------------------
    this->loop_pass_1();
    this->loop_drawScene();

    //Second pass
    //---------------------------
    this->loop_pass_2();
    this->loop_drawScreen();

    //GUI and end
    //---------------------------
    this->loop_gui();
    this->loop_end();
  }
  while(!glfwWindowShouldClose(window));

  //---------------------------
}
void CoreGLengine::loop_gui(){
  //---------------------------

  dimManager->set_is_resized(false);

  //Draw GUI on fbo 0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //engineManager->loop_gui();

  GUI_node* node_gui = engineManager->get_node_gui();
  GUI* guiManager = node_gui->get_guiManager();
  guiManager->Gui_loop();

  //---------------------------
}
void CoreGLengine::loop_pass_1(){
  //---------------------------

  //Update things
  dimManager->update();
  this->flag_resized = dimManager->get_is_resized();
  if(flag_resized){
    renderManager->update_texture();
    shaderManager->update();
  }

  //Set FBO
  renderManager->render_fbo_1();

  //Set active shader
  shaderManager->use("scene");
  mat4 mvp = cameraManager->compute_mvpMatrix();
  ShaderObject* shader_scene = shaderManager->get_shader_scene();
  shader_scene->setMat4("MVP", mvp);

  //---------------------------
}
void CoreGLengine::loop_pass_2(){
  //---------------------------


  renderManager->render_fbo_2();

  //Set active shader
  shaderManager->use("screen");

  //---------------------------
}
void CoreGLengine::loop_drawScene(){
  //---------------------------

  cameraManager->viewport_update(0);
  cameraManager->input_cameraMouseCommands();
  engineManager->loop_scene();

  //---------------------------
}
void CoreGLengine::loop_drawScreen(){
  //---------------------------

  //Viewport
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);

  //Update OpenGL quad window
  if(flag_resized){
    renderManager->update_quad();
  }
  //Draw screen quad
  else{
    renderManager->render_quad();
  }

  //---------------------------
}
void CoreGLengine::loop_end(){
  //---------------------------

  glfwSwapBuffers(window);
  glfwPollEvents();
  if(waitForEvent){
    glfwWaitEvents();
  }

  //---------------------------
}
