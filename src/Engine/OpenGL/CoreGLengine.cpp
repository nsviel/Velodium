#include "CoreGLengine.h"

#include "Camera.h"
#include "Viewport.h"
#include "Framebuffer.h"

#include "../Engine.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderObject.h"
#include "../Configuration/Dimension.h"
#include "../Configuration/Configuration.h"

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

  this->init_config();
  this->init_OGL();
  this->init_object();
  this->init_fbo();
  this->init_quad();
  this->init_shader();

  //---------------------------
  console.AddLog("sucess" ,"Program initialized...");
  return true;
}
bool CoreGLengine::init_config(){
  this->configManager = new Configuration();
  //---------------------------

  this->configManager->make_configuration();
  float backgColor = configManager->parse_json_float("window", "background_color");
  this->backgColor = vec3(backgColor, backgColor, backgColor);

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

  this->dimManager = new Dimension(window);
  this->shaderManager = new Shader(dimManager);
  this->cameraManager = new Camera(dimManager);
  this->viewportManager = new Viewport(dimManager);
  this->engineManager = new Engine(dimManager, cameraManager, shaderManager);
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
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}
void CoreGLengine::init_fbo(){
  dimManager->update_window_dim();
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Init FBO 1
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);

  //Init textures
  glGenTextures(1, &tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_ID, 0);

  glGenTextures(1, &tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_depth_ID, 0);

  //Init FBO 2
  glGenTextures(1, &tex_edl_ID);
  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, gl_dim.x+10000, gl_dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID, 0);

  //Debind objects
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
bool CoreGLengine::init_shader(){
  //---------------------------

  shaderManager->init();

  //---------------------------
  return true;
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
    this->loop_drawQuad();

    //GUI and end
    //---------------------------
    guiManager->Gui_loop();
    this->loop_end();
  }
  while(!glfwWindowShouldClose(window));

  //---------------------------
}
void CoreGLengine::loop_pass_1(){
  //---------------------------

  //Check for window resizing
  dimManager->update();
  flag_resized = dimManager->get_is_resized();

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  this->update_shader();
  this->update_texture();

  //Set active shader
  shaderManager->use("scene");
  mat4 mvp = cameraManager->compute_mvpMatrix();
  ShaderObject* shader_scene = shaderManager->get_shader_scene();
  shader_scene->setMat4("MVP", mvp);

  //---------------------------
}
void CoreGLengine::loop_pass_2(){
  //---------------------------

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);
  glDisable(GL_DEPTH_TEST);

  //Set active shader
  shaderManager->use("screen");

  //Set active textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);

  //---------------------------
}
void CoreGLengine::loop_drawScene(){
  //---------------------------

  cameraManager->viewport_update(0);
  cameraManager->input_cameraMouseCommands();
  engineManager->loop();

  //---------------------------
}
void CoreGLengine::loop_drawQuad(){
  //---------------------------

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(backgColor.x, backgColor.y, backgColor.z, 1.0f);

  //Viewport
  vec2 win_dim = dimManager->get_win_dim();
  glViewport(0, 0, win_dim[0], win_dim[1]);

  //Update OpenGL quad window
  this->update_gl_quad();

  //Draw screen quad
  if(!flag_resized){
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  //---------------------------
}
void CoreGLengine::loop_end(){
  bool waitForEvent = configManager->parse_json_bool("opengl", "waitForEvent");
  //---------------------------

  dimManager->set_is_resized(false);

  glfwSwapBuffers(window);
  glfwPollEvents();
  if(waitForEvent){
    glfwWaitEvents();
  }

  //---------------------------
}

void CoreGLengine::update_gl_quad(){
  //---------------------------

  if(flag_resized){
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
  }

  //---------------------------
}
void CoreGLengine::update_texture(){
  //---------------------------

  if(flag_resized){
    //Update texture dimensions
    vec2 gl_dim = dimManager->get_gl_dim();
    glBindTexture(GL_TEXTURE_2D, tex_color_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  }

  //---------------------------
}
void CoreGLengine::update_shader(){
  //---------------------------

  if(flag_resized){
    shaderManager->update();
  }

  //---------------------------
}
