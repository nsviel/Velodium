#ifndef MODULE_RADIOMETRY_H
#define MODULE_RADIOMETRY_H

#include "../../common.h"

class Module_node;
class Engine_node;
class Module_node;
class Node_operation;
class Load_node;


class Module_radiometry
{
public:
  //Constructor / Destructor
  Module_radiometry(Module_node* node_module);
  ~Module_radiometry();

public:
  void init();
  void update();
  void runtime();
  void reset();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}
  inline Load_node* get_node_load(){return node_load;}

private:
  Engine_node* node_engine;
  Module_node* node_module;
  Node_operation* node_ope;
  Load_node* node_load;
};

#endif
