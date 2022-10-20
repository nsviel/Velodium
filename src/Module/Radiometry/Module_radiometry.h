#if !defined(MODULE_RADIOMETRY_H)
#define MODULE_RADIOMETRY_H

#include "../../common.h"

class Node_module;
class Node_engine;
class Node_module;
class Node_operation;
class Node_load;


class Module_radiometry
{
public:
  //Constructor / Destructor
  Module_radiometry(Node_module* node_module);
  ~Module_radiometry();

public:
  void init();
  void update();
  void runtime();
  void reset();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_module* get_node_module(){return node_module;}
  inline Node_load* get_node_load(){return node_load;}

private:
  Node_engine* node_engine;
  Node_module* node_module;
  Node_operation* node_ope;
  Node_load* node_load;
};

#endif
