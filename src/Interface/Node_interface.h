#ifndef NODE_INTERFACE_H
#define NODE_INTERFACE_H

#include "../common.h"

class Node_engine;
class Node_load;
class Node_gui;

class Capture;
class MQTT;
class HTTP;

class GUI_Capture;
class GUI_Network;


class Node_interface
{
public:
  //Constructor / Destructor
  Node_interface(Node_engine* engine);
  ~Node_interface();

public:
  void update();
  void runtime();
  void draw();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_load* get_node_load(){return node_load;}
  inline Node_gui* get_node_gui(){return node_gui;}

  inline Capture* get_captureManager(){return captureManager;}
  inline MQTT* get_mqttManager(){return mqttManager;}
  inline HTTP* get_httpsManager(){return httpsManager;}

private:
  Node_engine* node_engine;
  Node_load* node_load;
  Node_gui* node_gui;

  MQTT* mqttManager;
  HTTP* httpsManager;
  Capture* captureManager;

  GUI_Network* gui_network;
  GUI_Capture* gui_capture;
};

#endif
