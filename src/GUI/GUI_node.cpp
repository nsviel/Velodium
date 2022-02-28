#include "GUI_node.h"

#include "GUI.h"
#include "Interface/GUI_Consol.h"
#include "Interface/GUI_MenuBar.h"
#include "Interface/GUI_LeftPanel.h"
#include "Interface/struct_consol.h"
#include "Control/GUI_Control.h"
#include "Control/GUI_Option.h"
#include "Control/GUI_FileManager.h"
#include "Control/GUI_Initialization.h"
#include "Windows/GUI_windows.h"

#include "../Module/Module_GUI.h"
#include "../Module/Module_node.h"
#include "../Engine/Engine_node.h"
#include "../Operation/Operation_node.h"


//Constructor / Destructor
GUI_node::GUI_node(Engine_node* engine){
  this->node_engine = engine;
  //---------------------------

  this->node_ope = node_engine->get_node_ope();
  this->node_module = node_engine->get_node_module();

  this->gui_initManager = new GUI_Initialization(this);
  this->gui_moduleManager = new GUI_module(this);
  this->gui_winManager = new GUI_windows(this);
  this->gui_controlManager = new GUI_control(this);
  this->gui_optionManager = new GUI_option(this);
  this->gui_fileManager = new GUI_fileManager(this);
  this->gui_leftPanelManager = new GUI_leftPanel(this);
  this->gui_menuBarManager = new GUI_menuBar(this);
  this->gui_consolManager = new GUI_consol(this);
  this->guiManager = new GUI(this);

  //---------------------------
}
GUI_node::~GUI_node(){}

//Main functions
void GUI_node::reset(){
  //---------------------------

  node_engine->reset();
  node_module->reset();

  //---------------------------
}
void GUI_node::update(){}
void GUI_node::runtime(){
  //---------------------------

  //gui_moduleManager->runtime();sayHello();

  //---------------------------
}
void GUI_node::exit(){
  //---------------------------

  node_engine->exit();

  //---------------------------
}
