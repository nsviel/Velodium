#include "CoreGLengine.h"

#include "Dimension.h"

#include "Camera/Camera.h"
#include "Camera/Viewport.h"
#include "Camera/Renderer.h"
#include "Shader/Shader.h"
#include "Shader/ShaderObject.h"

#include "../Engine.h"
#include "../Engine_node.h"
#include "../Scene/Configuration.h"

#include "../../GUI/GUI_node.h"
#include "../../GUI/GUI.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES


//Constructor / Destructor
CoreGLengine::CoreGLengine(){
  //---------------------------

  this->openglRunning = true;
  this->window = nullptr;

  //---------------------------
}
CoreGLengine::~CoreGLengine(){
  //---------------------------

  glfwDestroyWindow(window);
  glfwTerminate();

  //---------------------------
}

//Argument processing
void CoreGLengine::arg(int argc, char* argv[]){
  this->configManager = new Configuration();
  //---------------------------

  //Command line processing
  if(argc > 1){
    string command_1 = argv[1];

    if(command_1 == "car"){
      configManager->make_preconfig(1);
      node_engine->update();
    }
    else if(command_1 == "train"){
      configManager->make_preconfig(2);
      node_engine->update();
    }
  }
  //Else make default configuration
  else{
    configManager->make_preconfig(0);
  }

  //---------------------------
}

//Init opengl stuff
void CoreGLengine::init(){
  //---------------------------

  this->init_OGL();
  this->init_object();
  this->init_rendering();

  //---------------------------
  console.AddLog("sucess" ,"Program initialized...");
}
void CoreGLengine::init_OGL(){
  //---------------------------

  //Parametrization
  int resolution_width = configManager->parse_json_i("window", "resolution_width");
  int resolution_height = configManager->parse_json_i("window", "resolution_height");
  bool forceVersion = configManager->parse_json_b("opengl", "forceVersion");
  bool coreGL_verbose = configManager->parse_json_b("opengl", "verbose_coreGL");
  string win_title = configManager->parse_json_s("window", "title");
  this->is_visualization = configManager->parse_json_b("window", "visualization");

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
}
void CoreGLengine::init_object(){
  //---------------------------

  this->node_engine = new Engine_node(configManager, window);
  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->renderManager = node_engine->get_renderManager();
  this->engineManager = node_engine->get_engineManager();

  //---------------------------
}
void CoreGLengine::init_rendering(){
  if(is_visualization){
    //---------------------------

    renderManager->init_rendering_fbo_1();
    renderManager->init_rendering_fbo_2();
    renderManager->init_rendering_quad();
    shaderManager->init();

    //---------------------------
  }
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
  while(openglRunning);

  //---------------------------
}
void CoreGLengine::loop_gui(){
  //---------------------------

  dimManager->set_is_resized(false);

  //Draw GUI on fbo 0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  engineManager->loop_gui();

  //---------------------------
}
void CoreGLengine::loop_pass_1(){
  if(is_visualization){
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
}
void CoreGLengine::loop_pass_2(){
  if(is_visualization){
    //---------------------------

    //Framebuffer pass 2
    renderManager->render_fbo_2();

    //Set active shader
    shaderManager->use("screen");

    //---------------------------
  }
}
void CoreGLengine::loop_drawScene(){
  //---------------------------

  if(is_visualization){
    cameraManager->viewport_update(0);
    cameraManager->input_cameraMouseCommands();
  }

  engineManager->loop_scene();

  //---------------------------
}
void CoreGLengine::loop_drawScreen(){
  if(is_visualization){
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
}
void CoreGLengine::loop_end(){
  //---------------------------

  //Window display stuff
  glfwSwapBuffers(window);
  glfwPollEvents();

  //Check for window termination
  if(glfwWindowShouldClose(window)){
    openglRunning = false;
  }

  //---------------------------
}
