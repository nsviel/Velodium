#ifndef MODULE_OBSTACLE_H
#define MODULE_OBSTACLE_H

#include "../../common.h"

class Module_node;
class Engine_node;
class Node_operation;

class Obstacle;
class Filemanager;
class Configuration;
class Prediction;
class Warning;
class HTTP;
class MQTT;
class GUI_Obstacle;


class Module_obstacle
{
public:
  //Constructor / Destructor
  Module_obstacle(Module_node* node_module);
  ~Module_obstacle();

public:
  void init();
  void update();
  void runtime();
  void reset();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}

  inline Module_node* get_node_module(){return node_module;}
  inline Configuration* get_configManager(){return configManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Filemanager* get_fileManager(){return fileManager;}
  inline Prediction* get_predManager(){return predManager;}
  inline Warning* get_warningManager(){return warningManager;}
  inline MQTT* get_mqttManager(){return mqttManager;}
  inline HTTP* get_httpsManager(){return httpsManager;}

private:
  Engine_node* node_engine;
  Node_operation* node_ope;

  GUI_Obstacle* gui_obstacle;
  Configuration* configManager;
  Module_node* node_module;
  Obstacle* obstacleManager;
  Filemanager* fileManager;
  Prediction* predManager;
  Warning* warningManager;
  MQTT* mqttManager;
  HTTP* httpsManager;
};

#endif
