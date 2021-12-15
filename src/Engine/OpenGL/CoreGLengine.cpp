#include "CoreGLengine.h"

#include "Camera.h"
#include "Viewport.h"
#include "Framebuffer.h"

#include "../Engine.h"
#include "../Shader/Shader.h"
#include "../Configuration/Dimension.h"
#include "../Configuration/Configuration.h"

#include "../../GUI/GUI.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES


//Constructor / Destructor
CoreGLengine::CoreGLengine(){
  //---------------------------

  this->configManager = new Configuration();

  this->loop_cpt = 0;

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
  //glfwWindowHint(GLFW_SAMPLES, configuration.WINDOW_MultiSample);
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
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetWindowTitle(window, configuration.WINDOW_Title);

  //OpenGL stuff
  glViewport(0, 0, gl_width, gl_height);
  glPointSize(1);
  glLineWidth(1);
  /*glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LESS);*/

  //GLEW
  glewInit();
  if(configuration.VERBOSE_coreGL){
    std::cout << "GLEW initiated" << std::endl;
  }



  this->trucEDL();




  //---------------------------
  return true;
}
bool CoreGLengine::init_shader(){
  //---------------------------

  mvpShader = new Shader("../src/Engine/Shader/shader_mvp.vs", "../src/Engine/Shader/shader_mvp.fs");
  edlShader = new Shader("../src/Engine/Shader/shader_edl.vs", "../src/Engine/Shader/shader_edl.fs");
  fboShader = new Shader("../src/Engine/Shader/shader_fbo.vs", "../src/Engine/Shader/shader_fbo.fs");

  //---------------------------
  return true;
}
bool CoreGLengine::init_object(){
  //---------------------------

  this->dimManager = new Dimension(window);
  this->cameraManager = new Camera(dimManager);
  this->viewportManager = new Viewport(dimManager);
  this->engineManager = new Engine(dimManager, cameraManager);
  this->fboManager = new Framebuffer();
  this->guiManager = new GUI(engineManager);
  guiManager->Gui_bkgColor(&backgColor);

  //---------------------------
  return true;
}

void CoreGLengine::trucEDL(){
  int gl_width = configuration.WINDOW_InitResWidth;
  int gl_height = configuration.WINDOW_InitResHeight;





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

  //Quad stuff
  vector<vec2> quad_xy;
  quad_xy.push_back(vec2(-1.0f,  1.0f));
  quad_xy.push_back(vec2(-1.0f,  -1.0f));
  quad_xy.push_back(vec2(1.0f,  -1.0f));
  quad_xy.push_back(vec2(-1.0f,  1.0f));
  quad_xy.push_back(vec2(1.0f,  -1.0f));
  quad_xy.push_back(vec2(1.0f,  1.0f));

  vector<vec2> quad_uv;
  quad_uv.push_back(vec2(0.0f,  1.0f));
  quad_uv.push_back(vec2(0.0f,  0.0f));
  quad_uv.push_back(vec2(1.0f,  0.0f));
  quad_uv.push_back(vec2(0.0f,  1.0f));
  quad_uv.push_back(vec2(1.0f,  0.0f));
  quad_uv.push_back(vec2(1.0f,  1.0f));

  glGenBuffers(1, &quad_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad_xy.size()*sizeof(glm::vec2), &quad_xy[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glBufferData(GL_ARRAY_BUFFER, quad_uv.size()*sizeof(glm::vec2), &quad_uv[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glGenFramebuffers(1, &FBO);
  glGenTextures(1, &texture);

  do{
    vec2 dim = dimManager->get_glDim();int gl_width = dim.x;
    int gl_height = dim.y;


    //glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //FBO

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Texture


    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_width, gl_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);





    int nb_viewport = cameraManager->get_number_viewport();
    for(int i=0; i<1; i++){
      guiManager->Gui_loop();

      cameraManager->viewport_update(0);
      cameraManager->input_cameraMouseCommands();

      mvpShader->use();
      mat4 mvp = cameraManager->compute_mvpMatrix();
      mvpShader->setMat4("MVP", mvp);

      engineManager->loop();
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind your FBO to set the default framebuffer

    //glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);


    fboShader->use(); // shader program for rendering the quad

    glActiveTexture( GL_TEXTURE0);
    glBindVertexArray(quad_vao);
    glBindTexture(GL_TEXTURE_2D, texture); // color attachment texture
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





    /*
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    edlShader->use();




    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, texture_color_ID);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/

    glfwSwapBuffers(window);
    glfwPollEvents();
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
void CoreGLengine::loop_camera(int viewport_ID){
  //---------------------------

  cameraManager->viewport_update(viewport_ID);
  cameraManager->input_cameraMouseCommands();

  //---------------------------
}
void CoreGLengine::loop_shader(){
  //---------------------------

  mat4 mvp = cameraManager->compute_mvpMatrix();
  mvpShader->setMat4("MVP", mvp);

  //---------------------------
}
void CoreGLengine::loop_end(){
  //---------------------------

  glfwSwapBuffers(window);
  if(configuration.GL_WaitForEvent) glfwWaitEvents();

  //---------------------------
}
