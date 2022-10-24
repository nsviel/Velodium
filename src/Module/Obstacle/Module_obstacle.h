#ifndef MODULE_OBSTACLE_H
#define MODULE_OBSTACLE_H

#include "../../common.h"

class Node_module;
class Node_engine;
class Node_operation;
class Node_interface;

class Obstacle;
class Filemanager;
class Configuration;
class Prediction;
class Warning;
class GUI_Obstacle;


class Module_obstacle
{
public:
  //Constructor / Destructor
  Module_obstacle(Node_module* node_module);
  ~Module_obstacle();

public:
  void init();
  void update();
  void runtime();
  void reset();
  void draw();
  void draw_online();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_module* get_node_module(){return node_module;}
  inline Node_interface* get_node_interface(){return node_interface;}

  inline Configuration* get_configManager(){return configManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Filemanager* get_fileManager(){return fileManager;}
  inline Prediction* get_predManager(){return predManager;}
  inline Warning* get_warningManager(){return warningManager;}

  inline GUI_Obstacle* get_gui_obstacle(){return gui_obstacle;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;
  Node_interface* node_interface;

  Configuration* configManager;
  Node_module* node_module;
  Obstacle* obstacleManager;
  Filemanager* fileManager;
  Prediction* predManager;
  Warning* warningManager;

  GUI_Obstacle* gui_obstacle;
};

#endif
