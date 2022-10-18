#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "../common.h"

class Engine_node;
class Node_operation;

class Module_radiometry;
class Module_registration;
class Module_obstacle;
class Module_slam;


class Module_node
{
public:
  //Constructor / Destructor
  Module_node(Engine_node* engine);
  ~Module_node();

public:
  //Main functions
  void reset();
  void update();
  void runtime();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}

  inline Module_radiometry* get_module_radio(){return module_radio;}
  inline Module_registration* get_module_regist(){return module_regist;}
  inline Module_obstacle* get_module_obstacle(){return module_obstacle;}
  inline Module_slam* get_module_slam(){return module_slam;}

private:
  Engine_node* node_engine;
  Node_operation* node_ope;

  Module_registration* module_regist;
  Module_radiometry* module_radio;
  Module_obstacle* module_obstacle;
  Module_slam* module_slam;
};

#endif
