#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "../common.h"

class Node_engine;
class Node_operation;

class Module_radiometry;
class Module_registration;
class Module_obstacle;
class Module_slam;


class Node_module
{
public:
  //Constructor / Destructor
  Node_module(Node_engine* engine);
  ~Node_module();

public:
  //Main functions
  void load_module();
  void reset();
  void update();
  void runtime();
  void draw();
  void online(Cloud* cloud, int subset_ID);

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}

  inline Module_radiometry* get_module_radio(){return module_radio;}
  inline Module_registration* get_module_regist(){return module_regist;}
  inline Module_obstacle* get_module_obstacle(){return module_obstacle;}
  inline Module_slam* get_module_slam(){return module_slam;}

  inline bool* online_with_slam(){return &with_slam;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;

  Module_registration* module_regist;
  Module_radiometry* module_radio;
  Module_obstacle* module_obstacle;
  Module_slam* module_slam;

  bool with_slam;
};

#endif
