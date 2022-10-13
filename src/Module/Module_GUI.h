#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "../common.h"

class GUI_node;
class GUI_Player;
class GUI_Slam;
class GUI_Interface;
class GUI_operation;

class Module_node;
class Engine_node;
class Operation_node;


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(GUI_node* node);
  ~GUI_module();

public:
  void display_moduleTabs();
  void runtime();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}
  inline GUI_node* get_node_gui(){return node_gui;}
  
  inline GUI_Slam* get_gui_slam(){return gui_slam;}
  inline GUI_Player* get_gui_player(){return gui_player;}
  inline GUI_Interface* get_gui_interface(){return gui_interface;}
  inline GUI_operation* get_gui_operation(){return gui_operation;}

private:
  Module_node* node_module;
  Engine_node* node_engine;
  Operation_node* node_ope;

  GUI_node* node_gui;
  GUI_Player* gui_player;
  GUI_Slam* gui_slam;
  GUI_Interface* gui_interface;
  GUI_operation* gui_operation;

  bool module_velodyne;
  bool module_player;
  bool module_slam;
  bool module_obstacle;
  bool module_scala;
};

#endif
