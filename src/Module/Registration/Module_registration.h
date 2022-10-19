#ifndef MODULE_REGISTRATION_H
#define MODULE_REGISTRATION_H

#include "../../common.h"

class Engine_node;
class Node_operation;
class Module_node;


class Module_registration
{
public:
  //Constructor / Destructor
  Module_registration(Module_node* node_module);
  ~Module_registration();

public:
  void init();
  void update();
  void runtime();
  void reset();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}

private:
  Engine_node* node_engine;
  Node_operation* node_ope;
  Module_node* node_module;
};

#endif
