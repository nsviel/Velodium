#include "Node_gui.h"

#include "Window/GUI_Consol.h"
#include "Window/GUI_MenuBar.h"
#include "Window/GUI_LeftPanel.h"
#include "Window/struct_consol.h"

#include "Control/GUI.h"
#include "Control/GUI_Control.h"
#include "Control/GUI_Option.h"
#include "Control/GUI_FileManager.h"
#include "Control/GUI_Initialization.h"
#include "Control/GUI_Color.h"
#include "Control/GUI_Selection.h"

#include "Box/GUI_windows.h"

#include "Interface/GUI_Network.h"
#include "Interface/GUI_Capture.h"

#include "Dynamic/GUI_Online.h"
#include "Dynamic/GUI_Player.h"

#include "../Interface/Node_interface.h"
#include "../Module/Node_module.h"
#include "../Engine/Node_engine.h"
#include "../Operation/Node_operation.h"


//Constructor / Destructor
Node_gui::Node_gui(Node_engine* engine){
  this->node_engine = engine;
  //---------------------------

  this->node_ope = node_engine->get_node_ope();
  this->node_module = node_engine->get_node_module();
  this->node_interface = node_engine->get_node_interface();

  this->gui_initialization = new GUI_Initialization(this);
  this->gui_color = new GUI_Color(this);
  this->gui_window = new GUI_windows(this);
  this->gui_control = new GUI_control(this);
  this->gui_option = new GUI_option(this);
  this->gui_fileManager = new GUI_fileManager(this);
  this->gui_player = new GUI_Player(this);
  this->gui_menuBar = new GUI_menuBar(this);
  this->gui_consol = new GUI_consol(this);
  this->gui_online = new GUI_Online(this);
  this->gui_network = new GUI_Network(this);
  this->gui_capture = new GUI_Capture(this);
  this->gui_leftPanel = new GUI_leftPanel(this);
  this->gui_selection = new GUI_Selection(this);

  this->guiManager = new GUI(this);

  //---------------------------
}
Node_gui::~Node_gui(){}

//Main functions
void Node_gui::reset(){
  //---------------------------

  node_engine->reset();
  node_module->reset();

  //---------------------------
}
void Node_gui::exit(){
  //---------------------------

  node_engine->exit();

  //---------------------------
}
void Node_gui::loop(){
  //---------------------------

  guiManager->Gui_loop();

  //---------------------------
}
void Node_gui::loop_selection(){
  //---------------------------

  gui_selection->control_mouse();

  //---------------------------
}
