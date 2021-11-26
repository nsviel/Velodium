#include "Dimension.h"

//Constructor / Destructor
Dimension::Dimension(GLFWwindow* Window){
  this->window = Window;
  //---------------------------

  this->gui_leftPanel_dim = vec2(configuration.GUI_LeftPanel_width, 0);
  this->gui_topPanel_dim = vec2(0, configuration.GUI_TopPanel_height);
  this->gui_bottomPanel_dim = vec2(0, configuration.GUI_BotPanel_height);

  this->viewport_dim.x = configuration.WINDOW_InitResWidth - configuration.GUI_LeftPanel_width;
  this->viewport_dim.y = configuration.WINDOW_InitResHeight - configuration.GUI_TopPanel_height - configuration.GUI_BotPanel_height;

  this->viewport_pos = vec2(configuration.GUI_LeftPanel_width, configuration.GUI_BotPanel_height);

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

  configuration.GUI_LeftPanel_width = gui_leftPanel_dim.x;
  configuration.GUI_TopPanel_height = gui_topPanel_dim.y;
  configuration.GUI_BotPanel_height = gui_bottomPanel_dim.y;

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
