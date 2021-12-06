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



  this->trucEDL();




  //---------------------------
  return true;
}
bool CoreGLengine::init_shader(){
  //---------------------------

  //MVP shader
  /*this->shaderManager = new Shader();
  shaderManager->shader_build("shader_mvp");
  //shaderManager->shader_build("shader_EDL");
  shaderManager->run();*/

  shaderManager = new Shader("../src/Engine/Shader/framebuffer.vs", "../src/Engine/Shader/framebuffer.fs");
  screenShader = new Shader("../src/Engine/Shader/framebuffer_screen.vs", "../src/Engine/Shader/framebuffer_screen.fs");

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

void CoreGLengine::trucEDL(){
  int gl_width = configuration.WINDOW_InitResWidth;
  int gl_height = configuration.WINDOW_InitResHeight;




  /*glGenFramebuffers(1, &fbo_pass_1);
  glGenFramebuffers(1, &fbo_pass_2);
  glGenTextures(1, &texture_color_ID);
  glGenTextures(1, &texture_depth_ID);
  glGenTextures(1, &texture_postProcess_ID);

  /*double edl_strength = 100.0;
  double edl_distance = 1.0;
  double z_far = 10000.0;
  double z_near = 0.1;

  GLuint program_ID = shaderManager->get_program_ID();

  auto a_loc = glGetUniformLocation(program_ID, "A");
  auto b_loc = glGetUniformLocation(program_ID, "B");
  auto a = -(z_far + z_near) / (z_far - z_near);
  auto b = -2 * (z_far * z_near) / (z_far - z_near);
  glUniform1f(a_loc, (float) a);
  glUniform1f(b_loc, (float) b);

  auto edl_strength_loc = glGetUniformLocation(program_ID, "EDL_STRENGTH");
  auto edl_dist_loc = glGetUniformLocation(program_ID, "EDL_DISTANCE");
  auto with_edl_loc = glGetUniformLocation(program_ID, "WITH_EDL");

  glUniform1f(edl_strength_loc, (float) edl_strength);
  glUniform1f(edl_dist_loc, (float) edl_distance);
  glUniform1i(with_edl_loc, true);*/


  //Quad stuff
  float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
  };
  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


  // shader configuration
  // --------------------
  shaderManager->use();
  shaderManager->setInt("texture1", 0);

  screenShader->use();
  screenShader->setInt("screenTexture", 0);


      glGenFramebuffers(1, &fbo_pass_1);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo_pass_1);
      // create a color attachment texture
      unsigned int textureColorbuffer;
      glGenTextures(1, &textureColorbuffer);
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_width, gl_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
      // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
      unsigned int rbo;
      glGenRenderbuffers(1, &rbo);
      glBindRenderbuffer(GL_RENDERBUFFER, rbo);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gl_width, gl_height); // use a single renderbuffer object for both a depth AND stencil buffer.
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
      // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
          cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      // draw as wireframe
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

    glfwPollEvents();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);



    this->loop_camera();

    shaderManager->use();
    mat4 mvp = cameraManager->compute_mvpMatrix();
    shaderManager->setMat4("MVP", mvp);

    //this->loop_shader();
    engineManager->loop();
    guiManager->Gui_loop();


    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    screenShader->use();




    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, texture_color_ID);
    glDrawArrays(GL_TRIANGLES, 0, 6);*/

    glfwSwapBuffers(window);
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
  shaderManager->setMat4("MVP", mvp);

  //---------------------------
}
void CoreGLengine::loop_end(){
  //---------------------------

  glfwSwapBuffers(window);
  if(configuration.GL_WaitForEvent) glfwWaitEvents();

  //---------------------------
}
