#ifndef MODULE_REGISTRATION_H
#define MODULE_REGISTRATION_H

#include "../../common.h"

class Node_engine;
class Node_operation;
class Node_module;


class Module_registration
{
public:
  //Constructor / Destructor
  Module_registration(Node_module* node_module);
  ~Module_registration();

public:
  void init();
  void update();
  void runtime();
  void reset();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_module* get_node_module(){return node_module;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;
  Node_module* node_module;
};

#endif
