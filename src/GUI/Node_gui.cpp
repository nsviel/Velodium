#include "Node_gui.h"

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
  this->gui_window = new GUI_windows(this);
  this->gui_control = new GUI_control(this);
  this->gui_option = new GUI_option(this);
  this->gui_fileManager = new GUI_fileManager(this);
  this->gui_leftPanel = new GUI_leftPanel(this);
  this->gui_menuBar = new GUI_menuBar(this);
  this->gui_consol = new GUI_consol(this);
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
void Node_gui::update(){}
void Node_gui::runtime(){
  //---------------------------

  //---------------------------
}
void Node_gui::exit(){
  //---------------------------

  node_engine->exit();

  //---------------------------
}
void Node_gui::draw_leftPanel(){
  if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
    ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(0, 0, 0, 255));
    //-------------------------------

    node_ope->draw();
    node_module->draw();
    node_interface->draw();

    //-------------------------------
    ImGui::PopStyleColor();
    ImGui::EndTabBar();
  }
}
