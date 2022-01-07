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
  float backgColor = configManager->parse_json_float("window", "background_color");
  this->backgColor = vec3(backgColor, backgColor, backgColor);
  //---------------------------

  this->init_OGL();
  this->init_shader();
  this->init_object();

  //---------------------------
  return true;
}
bool CoreGLengine::init_OGL(){
  //---------------------------

  //Dimension
  int resolution_width = configManager->parse_json_int("window", "resolution_width");
  int resolution_height = configManager->parse_json_int("window", "resolution_height");
  int leftPanel_width = configManager->parse_json_int("gui", "leftPanel_width");
  int topPanel_height = configManager->parse_json_int("gui", "topPanel_height");

  gl_width = resolution_width - leftPanel_width;
  gl_height = resolution_height -topPanel_height;

  //GLFW
  bool forceVersion = configManager->parse_json_bool("opengl", "forceVersion");
  bool coreGL_verbose = configManager->parse_json_bool("opengl", "verbose_coreGL");
  int nb_multisample = configManager->parse_json_int("opengl", "nb_multisample");
  string win_title = configManager->parse_json_string("window", "title");

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

  //OpenGL stuff
  glPointSize(1);
  glLineWidth(1);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LESS);

  //GLEW
  glewInit();
  if(coreGL_verbose){
    std::cout << "GLEW initiated" << std::endl;
  }

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
void CoreGLengine::init_quad(){
  //---------------------------

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

  glGenVertexArrays(1, &quad_vao);
  glBindVertexArray(quad_vao);

  glGenBuffers(1, &quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad_xy.size()*sizeof(glm::vec2), &quad_xy[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  GLuint quad_vbo_uv;
  glGenBuffers(1, &quad_vbo_uv);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_uv);
  glBufferData(GL_ARRAY_BUFFER, quad_uv.size()*sizeof(glm::vec2), &quad_uv[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  //---------------------------
}
void CoreGLengine::init_fbo(){
  //---------------------------

  dimManager->update_window_dim();
  vec2 gl_dim = dimManager->get_gl_dim();

  //Init texture
  glGenTextures(1, &texture_ID);
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D ,0);

  //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_ID);
  //glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, gl_width, gl_height, GL_TRUE);
  //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  //Init FBO
  glGenFramebuffers(1, &fbo_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_ID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ID, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture_ID, 0);
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_ID);

  //---------------------------
}

void CoreGLengine::loop(){
  //---------------------------

  this->init_fbo();
  this->init_quad();

  do{
    //First pass
    //---------------------------
    this->loop_pass_1();

    //Drawing block
    mat4 mvp = cameraManager->compute_mvpMatrix();
    mvpShader->setMat4("MVP", mvp);
    cameraManager->viewport_update(0);
    cameraManager->input_cameraMouseCommands();
    engineManager->loop();

    //Second pass
    //---------------------------
    this->loop_pass_2();
    guiManager->Gui_loop();
    this->loop_end();
  }
  while(!glfwWindowShouldClose(window));

  //---------------------------
}
void CoreGLengine::loop_pass_1(){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_ID);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  vec2 gl_dim = dimManager->get_gl_dim();
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  mvpShader->use();

  //---------------------------
}
void CoreGLengine::loop_pass_2(){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

  //Viewport
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);

  //Update OpenGL quad window
  this->update_gl_quad();

  //Shader program for rendering the quad
  fboShader->use();

  //Quad & texture drawing
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(quad_vao);
  //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_ID);
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
  bool waitForEvent = configManager->parse_json_bool("opengl", "waitForEvent");
  //---------------------------

  glfwSwapBuffers(window);
  glfwPollEvents();
  if(waitForEvent){
    glfwWaitEvents();
  }

  //---------------------------
}

void CoreGLengine::update_gl_quad(){
  //---------------------------

  vec2 gl_pos = dimManager->get_gl_pos();
  vec2 gl_dim = dimManager->get_gl_dim();
  vec2 win_dim = dimManager->get_win_dim();

  vec2 tl, br, tr, bl;
  bl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  bl.y = 2 * (gl_pos.y) / (win_dim.y) - 1;

  br.x = 1;
  br.y = 2 * (gl_pos.y) / (win_dim.y) - 1;

  tl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  tl.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;

  tr.x = 1;
  tr.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;

  vector<vec2> quad_xy;
  quad_xy.push_back(tl);
  quad_xy.push_back(bl);
  quad_xy.push_back(br);

  quad_xy.push_back(tl);
  quad_xy.push_back(br);
  quad_xy.push_back(tr);

  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad_xy.size() * sizeof(glm::vec2), &quad_xy[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
