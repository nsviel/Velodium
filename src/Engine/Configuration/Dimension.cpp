#include "Dimension.h"

#include "Configuration.h"


//Constructor / Destructor
Dimension::Dimension(GLFWwindow* Window){
  this->window = Window;
  //---------------------------

  this->configManager = new Configuration();

  int resolution_height = configManager->parse_json_int("window", "resolution_height");
  int resolution_width = configManager->parse_json_int("window", "resolution_width");
  int leftPanel_width = configManager->parse_json_int("gui", "leftPanel_width");
  int topPanel_height = configManager->parse_json_int("gui", "topPanel_height");
  int botPanel_height = configManager->parse_json_int("gui", "botPanel_height");

  this->gui_leftPanel_width = leftPanel_width;
  this->gui_topPanel_height = topPanel_height;
  this->gui_bottomPanel_height = botPanel_height;

  this->gl_dim.x = resolution_width - leftPanel_width;
  this->gl_dim.y = resolution_height - topPanel_height - botPanel_height;
  this->gl_pos = vec2(leftPanel_width, botPanel_height);

  //---------------------------
  this->update_window_dim();
}
Dimension::~Dimension(){}

//Main functions
void Dimension::update_gl_dim(){
  //---------------------------

  int gl_width = window_dim.x - gui_leftPanel_width;
  int gl_height = window_dim.y - gui_topPanel_height - gui_bottomPanel_height;

  gl_dim = vec2(gl_width, gl_height);
  gl_pos = vec2(gui_leftPanel_width, gui_bottomPanel_height);

  //---------------------------
}
void Dimension::update_window_dim(){
  int width, height;
  //---------------------------

  glfwGetWindowSize(window, &width, &height);
  window_dim = vec2(width, height);

  this->update_gl_dim();
  this->update_configuration();

  //---------------------------
}
void Dimension::update_configuration(){
  //---------------------------

  configManager->update_jsonfile("gui", "leftPanel_width", to_string(gui_leftPanel_width));
  configManager->update_jsonfile("gui", "topPanel_height", to_string(gui_topPanel_height));
  configManager->update_jsonfile("gui", "botPanel_height", to_string(gui_bottomPanel_height));

  //---------------------------
}

//Subfunctions
vec2 Dimension::get_glMiddle(){
  //---------------------------

  int x = gui_leftPanel_width + gl_dim.x/2;
  int y = gui_topPanel_height + gl_dim.y/2;
  vec2 middle = vec2(x, y);

  //---------------------------
  return middle;
}
vec2 Dimension::get_cursorPos_gl(){
  double xpos, ypos;
  //---------------------------

  glfwGetCursorPos(window, &xpos, &ypos);
  xpos = xpos - gui_leftPanel_width;
  ypos = ypos - gui_topPanel_height;

  vec2 pos = vec2(xpos, ypos);

  //---------------------------
  return pos;
}
vec2 Dimension::get_cursorPos(){
  double xpos, ypos;
  //---------------------------

  glfwGetCursorPos(window, &xpos, &ypos);
  vec2 pos = vec2(xpos, ypos);

  //---------------------------
  return pos;
}
void Dimension::set_cursorPos(vec2 pos){
  //---------------------------

  glfwSetCursorPos(window, pos.x, pos.y);

  //---------------------------
}
