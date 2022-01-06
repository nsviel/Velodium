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

  this->gui_leftPanel_dim = vec2(leftPanel_width, 0);
  this->gui_topPanel_dim = vec2(0, topPanel_height);
  this->gui_bottomPanel_dim = vec2(0, botPanel_height);

  this->viewport_dim.x = resolution_width - leftPanel_width;
  this->viewport_dim.y = resolution_height - topPanel_height - botPanel_height;

  this->viewport_pos = vec2(leftPanel_width, botPanel_height);

  //---------------------------
  this->update_window_dim();
}
Dimension::~Dimension(){}

//Main functions
void Dimension::update_viewport_dim(){
  //---------------------------

  int width = window_dim.x - gui_leftPanel_dim.x;
  int height = window_dim.y - gui_topPanel_dim.y - gui_bottomPanel_dim.y;

  viewport_dim = vec2(width, height);
  viewport_pos = vec2(gui_leftPanel_dim.x, gui_bottomPanel_dim.y);

  //---------------------------
}
void Dimension::update_window_dim(){
  int width, height;
  //---------------------------

  glfwGetWindowSize(window, &width, &height);
  window_dim = vec2(width, height);



  this->update_viewport_dim();
  this->update_configuration();

  //---------------------------
}
void Dimension::update_configuration(){
  //---------------------------

  configManager->update_jsonfile("gui", "leftPanel_width", to_string(gui_leftPanel_dim.x));
  configManager->update_jsonfile("gui", "topPanel_height", to_string(gui_topPanel_dim.y));
  configManager->update_jsonfile("gui", "botPanel_height", to_string(gui_bottomPanel_dim.y));

  //---------------------------
}

//Subfunctions
vec2 Dimension::get_glMiddle(){
  //---------------------------

  int x = gui_leftPanel_dim.x + viewport_dim.x/2;
  int y = gui_topPanel_dim.y + viewport_dim.y/2;
  vec2 middle = vec2(x, y);

  //---------------------------
  return middle;
}
vec2 Dimension::get_cursorPos_gl(){
  double xpos, ypos;
  //---------------------------

  glfwGetCursorPos(window, &xpos, &ypos);
  xpos = xpos - gui_leftPanel_dim.x;
  ypos = ypos - gui_topPanel_dim.y;

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
