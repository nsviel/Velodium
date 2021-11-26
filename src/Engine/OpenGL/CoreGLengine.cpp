#include "CoreGLengine.h"

#include "Camera.h"
#include "Framebuffer.h"

#include "../Engine.h"
#include "../Shader/Shader.h"
#include "../Configuration/Dimension.h"
#include "../Configuration/Configuration.h"

#include "../../GUI/GUI.h"

#include <GL/glew.h>


#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES

//Constructor / Destructor
CoreGLengine::CoreGLengine(){
  //---------------------------

  this->configManager = new Configuration();

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
  this->configManager->make_configuration();
  float backgColor = configuration.WINDOW_BckgColor;
  this->backgColor = vec3(backgColor, backgColor, backgColor);
  //---------------------------

  this->init_OGL();
  this->init_shader();
  this->init_object();

  //---------------------------
  return true;
}
bool CoreGLengine::init_OGL(){
  int gl_width = configuration.WINDOW_InitResWidth - configuration.GUI_LeftPanel_width;
  int gl_height = configuration.WINDOW_InitResHeight - configuration.GUI_TopPanel_height;
  //---------------------------

  //GLFW
  glfwInit();
  if(configuration.GL_ForceVersion){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  }
  glfwWindowHint(GLFW_SAMPLES, configuration.WINDOW_MultiSample);
  glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  window = glfwCreateWindow(gl_width,gl_height,"window",NULL,NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }else if(configuration.VERBOSE_coreGL){;
    std::cout << "GLFW window created" << std::endl;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
  glfwSetWindowTitle(window, configuration.WINDOW_Title);

  //OpenGL stuff
  glViewport(0, 0, gl_width, gl_height/2);
  glPointSize(1);
  glLineWidth(1);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LESS);

  //GLEW
  glewInit();
  if(configuration.VERBOSE_coreGL){
    std::cout << "GLEW initiated" << std::endl;
  }

  //---------------------------
  return true;
}
bool CoreGLengine::init_shader(){
  //---------------------------

  //MVP shader
  this->mvp_shaderManager = new Shader();
  mvp_shaderManager->shader_build("shader_mvp");
  mvp_shaderManager->run();

  //---------------------------
  return true;
}
bool CoreGLengine::init_object(){
  //---------------------------

  this->dimManager = new Dimension(window);
  this->cameraManager = new Camera(dimManager);
  this->engineManager = new Engine(dimManager, cameraManager);
  this->fboManager = new Framebuffer();
  this->guiManager = new GUI(engineManager);
  guiManager->Gui_bkgColor(&backgColor);

  //---------------------------
  return true;
}

//Engine loop
/*void CoreGLengine::loop(){
  //---------------------------

  do{
    this->loop_begin();
    this->loop_camera();
    engineManager->loop();
    guiManager->Gui_loop();
    this->loop_shader();;
    this->loop_end();
  }
  while(!glfwWindowShouldClose(window));

  //---------------------------
  configManager->save_configuration();
}*/
void CoreGLengine::loop(){
  //---------------------------


  do{


    this->loop_begin();
    this->loop_camera();
    this->loop_shader();
    engineManager->loop();
    guiManager->Gui_loop();



    this->loop_end();
  }
  while(!glfwWindowShouldClose(window));

  //---------------------------
  configManager->save_configuration();
}
void CoreGLengine::loop_begin(){
  //---------------------------

  glfwPollEvents();
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //---------------------------
}
void CoreGLengine::loop_camera(){
  //---------------------------

  cameraManager->update_viewport();
  cameraManager->input_cameraMouseCommands();
  cameraManager->input_cameraKeyCommands();

  //---------------------------
}
void CoreGLengine::loop_shader(){
  //---------------------------

  mat4 mvp = cameraManager->compute_mvpMatrix();
  mvp_shaderManager->setMat4("MVP", mvp);

  //---------------------------
}
void CoreGLengine::loop_end(){
  //---------------------------

  glfwSwapBuffers(window);
  if(configuration.GL_WaitForEvent) glfwWaitEvents();

  //---------------------------
}
