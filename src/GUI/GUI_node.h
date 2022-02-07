#ifndef NODE_GUI_H
#define NODE_GUI_H

#include "GUI.h"
#include "Interface/GUI_Consol.h"
#include "Interface/GUI_MenuBar.h"
#include "Interface/GUI_LeftPanel.h"
#include "Interface/struct_consol.h"
#include "Node/GUI_Control.h"
#include "Node/GUI_Option.h"
#include "Node/GUI_FileManager.h"
#include "Windows/GUI_windows.h"
#include "../Module/Module_GUI.h"
#include "../Module/Module_GUI.h"

#include "../Module/Module_node.h"
#include "../Engine/Engine_node.h"
#include "../Engine/Configuration/Configuration_node.h"
#include "../Operation/Operation_node.h"

#include "../common.h"


class GUI_node
{
public:
  //Constructor / Destructor
  GUI_node(Engine_node* engine, Operation_node* ope, Module_node* module, Configuration_node* config){
    //---------------------------

    this->node_engine = engine;
    this->node_ope = ope;
    this->node_module = module;
    this->node_config = config;

    //---------------------------
    this->init_objects();
  }
  ~GUI_node(){}

public:
  //Main functions
  void init_objects(){
    //---------------------------

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
  void reset(){
    //---------------------------

    //---------------------------
  }
  void update(){}
  void runtime(){
    //---------------------------

    //---------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}
  inline Configuration_node* get_node_config(){return node_config;}

  inline GUI* get_guiManager(){return guiManager;}
  inline GUI_windows* get_gui_winManager(){return gui_winManager;}
  inline GUI_control* get_gui_controlManager(){return gui_controlManager;}
  inline GUI_option* get_gui_optionManager(){return gui_optionManager;}
  inline GUI_menuBar* get_gui_menuBarManager(){return gui_menuBarManager;}
  inline GUI_leftPanel* get_gui_leftPanelManager(){return gui_leftPanelManager;}
  inline GUI_consol* get_gui_consolManager(){return gui_consolManager;}
  inline GUI_module* get_gui_moduleManager(){return gui_moduleManager;}
  inline GUI_fileManager* get_gui_fileManager(){return gui_fileManager;}

private:
  Engine_node* node_engine;
  Operation_node* node_ope;
  Module_node* node_module;
  Configuration_node* node_config;

  GUI* guiManager;
  GUI_windows* gui_winManager;
  GUI_control* gui_controlManager;
  GUI_option* gui_optionManager;
  GUI_menuBar* gui_menuBarManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_consol* gui_consolManager;
  GUI_module* gui_moduleManager;
  GUI_fileManager* gui_fileManager;
};

#endif
