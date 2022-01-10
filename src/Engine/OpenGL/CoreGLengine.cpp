#include "CoreGLengine.h"

#include "Camera.h"
#include "Viewport.h"
#include "Framebuffer.h"

#include "../Engine.h"
#include "../Shader/Shader.h"
#include "../Shader/PP_edl.h"
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
  this->init_object();
  this->init_fbo();
  this->init_quad();
  this->init_shader();

  //---------------------------
  console.AddLog("sucess" ,"Program initialized...");
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
  //glEnable(GL_MULTISAMPLE);
  //glEnable(GL_BLEND);
  //glEnable(GL_DEPTH_TEST);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  shader_scene = new Shader("../src/Engine/Shader/shader_scene.vs", "../src/Engine/Shader/shader_scene.fs");
  //shader_screen = new Shader("../src/Engine/Shader/shader_screen.vs", "../src/Engine/Shader/shader_screen.fs");
  //shader_edl = new Shader("../src/Engine/Shader/shader_edl.vs", "../src/Engine/Shader/shader_edl.fs");
  shader_screen = new Shader("../src/Engine/Shader/shader_edl.vs", "../src/Engine/Shader/shader_edl.fs");


  edlManager->setup_textures(tex_color_ID, tex_depth_ID);
  edlManager->setup_edl(shader_screen);

  //Integration en cours
  //voir fbo_2_ID
  //voir appel aux texture dans edl

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
  this->edlManager = new PP_edl();
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
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}
void CoreGLengine::init_fbo(){
  //---------------------------

  dimManager->update_window_dim();
  vec2 gl_dim = dimManager->get_gl_dim();

  //Init textures
  glGenTextures(1, &texture_ID);
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D ,0);

  glGenTextures(1, &tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D ,0);

  glGenTextures(1, &tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D ,0);

  glGenTextures(1, &tex_edl_ID);
  glBindTexture(GL_TEXTURE_2D, tex_edl_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D ,0);

  //Init FBO
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_ID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_depth_ID, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_edl_ID, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}

void CoreGLengine::loop(){
  //---------------------------

  do{
    //First pass
    //---------------------------
    this->loop_pass_1();

    //Drawing block
    mat4 mvp = cameraManager->compute_mvpMatrix();
    shader_scene->setMat4("MVP", mvp);
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

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  vec2 gl_dim = dimManager->get_gl_dim();
  //glBindTexture(GL_TEXTURE_2D, texture_ID);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  shader_scene->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);


  //---------------------------
}
void CoreGLengine::loop_pass_2(){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);


  auto color_texture_loc = glGetUniformLocation(shader_screen->get_program_ID(), "tex_color");
  auto depth_texture_loc = glGetUniformLocation(shader_screen->get_program_ID(), "tex_depth");
  glUniform1i(color_texture_loc, tex_color_ID);
  glUniform1i(depth_texture_loc, tex_depth_ID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);


  //Shader program for rendering the quad
  shader_screen->use();


  //Viewport
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);

  //Update OpenGL quad window
  this->update_gl_quad();


  //Quad & texture drawing
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, texture_ID);


  //Draw screen quad
  glBindVertexArray(quad_vao);
  //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture_ID);
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
  shader_scene->setMat4("MVP", mvp);

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
