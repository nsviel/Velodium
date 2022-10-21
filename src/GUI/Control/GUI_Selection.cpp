#include "GUI_Selection.h"

#include "../Node_gui.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Function/Selection.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Dimension.h"

#include <GLFW/glfw3.h>


//Constructor / Destructor
GUI_Selection::GUI_Selection(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_engine* node_engine = node_gui->get_node_engine();

  this->dimManager = node_engine->get_dimManager();
  this->selectionManager = node_ope->get_selectionManager();

  //---------------------------
}
GUI_Selection::~GUI_Selection(){}


//Main function
void GUI_Selection::control_mouse(){
  GLFWwindow* window = dimManager->get_window();
  //----------------------------

  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  if(GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)sayHello();

  //---------------------------
}
