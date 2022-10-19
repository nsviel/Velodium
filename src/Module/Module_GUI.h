#ifndef GUI_MODULE_H
#define GUI_MODULE_H

#include "../common.h"

class GUI_node;
class GUI_Player;
class GUI_Slam;

class Module_node;
class Engine_node;
class Node_operation;


class GUI_module
{
public:
  //Constructor / Destructor
  GUI_module(GUI_node* node);
  ~GUI_module();

public:
  void display_moduleTabs();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}

private:
  Module_node* node_module;
  Engine_node* node_engine;
  Node_operation* node_ope;
};

#endif
