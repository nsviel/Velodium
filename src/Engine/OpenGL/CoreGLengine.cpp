#include "CoreGLengine.h"

#include "../GPU/GPU_render.h"
#include "../GPU/GPU_screenshot.h"
#include "../Node_engine.h"
#include "../Shader/Shader.h"
#include "../Shader/Base/Shader_object.h"
#include "../Camera/Camera.h"
#include "../Camera/Viewport.h"
#include "../Core/Argument.h"
#include "../Core/Dimension.h"
#include "../Core/Engine.h"
#include "../Core/Configuration.h"

#include "../../GUI/Node_gui.h"
#include "../../GUI/Control/GUI.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES

#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


//Constructor / Destructor
CoreGLengine::CoreGLengine(){
  //---------------------------

  this->configManager = new Configuration();

  this->loop_run = true;
  this->window = nullptr;
  this->flag_resized = false;

  //---------------------------
}
CoreGLengine::~CoreGLengine(){
  //---------------------------

  delete configManager;
  delete node_engine;

  glfwDestroyWindow(window);
  glfwTerminate();

  //---------------------------
}

//Opengl stuff
void CoreGLengine::arg(int argc, char* argv[]){
  Argument argManager(node_engine);
  //---------------------------

  argManager.process_arg(argc, argv);

  //---------------------------
}
void CoreGLengine::init(){
  //---------------------------

  this->init_opengl();
  this->init_object();
  this->init_rendering();

  //---------------------------
  console.AddLog("ok" ,"Program initialized...");
}
void CoreGLengine::loop(){
  //---------------------------

  do{
    auto t1 = high_resolution_clock::now();

    //First pass
    //---------------------------
    this->loop_pass_1();
    this->loop_draw_scene();
    this->loop_selection();

    //Second pass
    //---------------------------
    this->loop_pass_2();
    this->loop_draw_canvas();

    //GUI and end
    //---------------------------
    this->loop_gui();
    this->loop_end();

    //Time loop
    auto t2 = high_resolution_clock::now();
    this->time_loop = duration_cast<milliseconds>(t2 - t1).count();
  }
  while(loop_run);

  //---------------------------
}

// Initialization stuff
void CoreGLengine::init_opengl(){
  //---------------------------

  //Parametrization
  int resolution_width = configManager->parse_json_i("window", "resolution_width");
  int resolution_height = configManager->parse_json_i("window", "resolution_height");
  bool forceVersion = configManager->parse_json_b("window", "forceVersion");
  string win_title = configManager->parse_json_s("window", "title");

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

  // OpenGL face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //Enable shader point size setting
  //glEnable(GL_PROGRAM_POINT_SIZE);
  //Multisampling
  //glfwWindowHint(GLFW_SAMPLES, 4);
  //glEnable(GL_MULTISAMPLE);

  //GLEW
  glewInit();

  //---------------------------
}
void CoreGLengine::init_object(){
  //---------------------------

  this->node_engine = new Node_engine(this);
  this->node_gui = node_engine->get_node_gui();
  this->dimManager = node_engine->get_dimManager();;
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->viewportManager = node_engine->get_viewportManager();
  this->renderManager = node_engine->get_renderManager();
  this->engineManager = node_engine->get_engineManager();
  this->screenshotManager = node_engine->get_screenshotManager();

  //---------------------------
}
void CoreGLengine::init_rendering(){
  //---------------------------

  dimManager->update();
  shaderManager->init_shader_objects();
  renderManager->init_create_fbo(shaderManager->get_nb_shader());
  renderManager->init_create_canvas();

  //---------------------------
}

// Loop scene stuff
void CoreGLengine::loop_gui(){
  //---------------------------

  dimManager->set_is_resized(false);

  //Draw GUI on fbo 0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  node_gui->loop();

  //---------------------------
}
void CoreGLengine::loop_selection(){
  //---------------------------

  node_gui->loop_selection();

  //---------------------------
}
void CoreGLengine::loop_draw_scene(){
  //---------------------------

  //Camera & scene runtime
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);
  cameraManager->input_cam_mouse();
  node_engine->runtime();

  //Untextured cloud & glyph drawing
  shaderManager->use_shader("screen");
  cameraManager->update_shader();

  engineManager->draw_untextured_cloud();
  engineManager->draw_untextured_glyph();

  //Textured cloud drawing
  shaderManager->use_shader("texture");
  cameraManager->update_shader();

  engineManager->draw_textured_cloud();

  //---------------------------
}

// Loop rendering stuff
void CoreGLengine::loop_pass_1(){
  dimManager->update();
  //---------------------------

  //Update things
  this->flag_resized = dimManager->get_is_resized();
  if(flag_resized){
    renderManager->update_dim_texture();
    shaderManager->update_shader_objects();
  }

  //Set screen space FBO
  renderManager->bind_fbo_pass_1();

  //---------------------------
}
void CoreGLengine::loop_pass_2(){
  //---------------------------

  //Viewport
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);

  //Framebuffer pass 2
  //Set EDL shader
  shaderManager->use_shader("edl");
  renderManager->bind_fbo_pass_2_edl();



  shaderManager->use_shader("inversion");
  renderManager->bind_fbo_pass_2_inv();

  //---------------------------
}
void CoreGLengine::loop_draw_canvas(){
  //---------------------------



  //If window resizing, update canvas size
  if(flag_resized){
    renderManager->update_dim_canvas();
  }

  //Draw screen quad
  shaderManager->use_shader("last");
  renderManager->bind_canvas();

  //---------------------------
}
void CoreGLengine::loop_end(){
  //---------------------------

  //End, if needed, by a screenshot
  screenshotManager->render_screenshot_online();

  //Window display stuff
  glfwSwapBuffers(window);
  glfwPollEvents();

  //Check for window termination
  if(glfwWindowShouldClose(window)){
    loop_run = false;
  }

  //---------------------------
}
