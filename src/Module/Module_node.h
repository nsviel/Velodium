#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "../common.h"

class Engine_node;
class Operation_node;
class Slam;
class Interface_node;
class Player_node;


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
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Player_node* get_node_player(){return node_player;}

  inline Slam* get_slamManager(){return slamManager;}
  inline Interface_node* get_ioManager(){return ioManager;}

private:
  Engine_node* node_engine;
  Operation_node* node_ope;

  Slam* slamManager;
  Interface_node* ioManager;
  Player_node* node_player;
};

#endif
