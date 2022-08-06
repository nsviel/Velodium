#include "Dimension.h"

#include "../Scene/Configuration.h"


//Constructor / Destructor
Dimension::Dimension(GLFWwindow* win, Configuration* config){
  this->window = win;
  this->configManager = config;
  //---------------------------

  int win_h = configManager->parse_json_i("window", "resolution_height");
  int win_w = configManager->parse_json_i("window", "resolution_width");
  int lp_w = configManager->parse_json_i("gui", "leftPanel_width");
  int tp_h = configManager->parse_json_i("gui", "topPanel_height");
  int bp_h = configManager->parse_json_i("gui", "botPanel_height");
  int lp_m = configManager->parse_json_i("gui", "leftPanel_mid");
  this->is_visualization = configManager->parse_json_b("window", "visualization");

  this->gl_dim.x = win_w - lp_w;
  this->gl_dim.y = win_h - tp_h - bp_h;
  this->gl_pos = vec2(lp_w, bp_h);

  this->gui_ltp_dim = vec2(lp_w, lp_m - tp_h);
  this->gui_ltp_pos = vec2(0, tp_h);
  this->gui_lbp_dim = vec2(lp_w, win_h - lp_m);
  this->gui_lbp_pos = vec2(0, lp_m);
  this->gui_lp_mid = lp_m;
  this->gui_tp_dim = vec2(win_w, tp_h);
  this->gui_bp_pos = vec2(lp_w, win_h - bp_h);
  this->gui_bp_dim = vec2(win_w - lp_w, bp_h);

  this->is_resized = true;
  this->with_custom_gl_dim = false;

  //---------------------------
  this->update();
}
Dimension::~Dimension(){}

//Main functions
void Dimension::update(){
  //---------------------------

  this->update_opengl_dim();
  this->update_window_dim();
  this->update_gui_consol();

  //---------------------------
}
void Dimension::update_opengl_dim(){
  //---------------------------

  //Check GL size
  int gl_w = win_dim.x - gui_ltp_dim.x;
  int gl_h = win_dim.y - gui_tp_dim.y - gui_bp_dim.y;

  if(gl_w != gl_dim.x || gl_h != gl_dim.y && !with_custom_gl_dim){
    //Set up new values
    this->gl_dim = vec2(gl_w, gl_h);
    this->gl_pos = vec2(gui_ltp_dim.x, gui_bp_dim.y);
    this->is_resized = true;
  }

  //---------------------------
}
void Dimension::update_window_dim(){
  if(is_visualization){
    //---------------------------

    //Check window size
    int win_w = 0;
    int win_h = 0;
    glfwGetWindowSize(window, &win_w, &win_h);

    //Check for minimal dimension
    bool min_dim = false;
    if(win_w < 300){
      win_w = 300;
      min_dim = true;
    }
    if(win_h < 300){
      win_h = 300;
      min_dim = true;
    }
    if(min_dim){
      glfwSetWindowSize(window, win_w, win_h);
    }

    //Set up new values
    if(win_w != win_dim.x || win_h != win_dim.y){
      this->win_dim = vec2(win_w, win_h);
      this->gui_bp_pos = vec2(gui_lbp_dim.x, win_h - gui_bp_dim.y);
      this->gui_bp_dim.x = win_w - gui_lbp_dim.x;
      this->gui_ltp_dim.y = gui_lp_mid - gui_tp_dim.y;
      this->gui_lbp_dim.y = win_h - gui_lp_mid;
      this->is_resized = true;
    }

    //---------------------------
  }
}
void Dimension::update_gui_consol(){
  //---------------------------

  if(is_visualization == false){
    this->gui_bp_pos = vec2(0, 18);
    this->gui_bp_dim = vec2(win_dim.x, win_dim.y - 18);
  }

  //---------------------------
}
void Dimension::update_configuration(){
  //---------------------------

  configManager->update_jsonfile("gui", "leftPanel_width", to_string(gui_ltp_dim.x));
  configManager->update_jsonfile("gui", "topPanel_height", to_string(gui_tp_dim.y));
  configManager->update_jsonfile("gui", "botPanel_height", to_string(gui_bp_dim.y));

  //---------------------------
}

//Subfunctions
vec2 Dimension::get_gl_middle(){
  //---------------------------

  int x = gui_ltp_dim.x + gl_dim.x/2;
  int y = gui_tp_dim.y + gl_dim.y/2;
  vec2 gl_middle = vec2(x, y);

  //---------------------------
  return gl_middle;
}
vec2 Dimension::get_cursorPos_gl(){
  double xpos, ypos;
  vec2 pos = vec2(0, 0);
  //---------------------------

  glfwGetCursorPos(window, &xpos, &ypos);
  xpos = xpos - gui_ltp_dim.x;
  ypos = ypos - gui_tp_dim.y;

  pos = vec2(xpos, ypos);

  //---------------------------
  return pos;
}
vec2 Dimension::get_mouse_pose(){
  double xpos, ypos;
  vec2 pos = vec2(0, 0);
  //---------------------------

  glfwGetCursorPos(window, &xpos, &ypos);
  pos = vec2(xpos, ypos);

  //---------------------------
  return pos;
}
void Dimension::set_mouse_pose(vec2 pos){
  //---------------------------

  glfwSetCursorPos(window, pos.x, pos.y);

  //---------------------------
}
