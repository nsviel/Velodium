#ifndef NODE_INTERFACE_H
#define NODE_INTERFACE_H

#include "../Specific/Base/Node.h"

class Node_engine;
class Node_load;
class Node_gui;
class Node_operation;

class Configuration;
class Capture;
class Recorder;
class HTTP;
class PCAP_reader;

class GUI_Capture;
class GUI_Network;


class Node_interface : public Node
{
public:
  //Constructor / Destructor
  Node_interface(Node_engine* engine);
  ~Node_interface();

public:
  void update();
  void runtime();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_load* get_node_load(){return node_load;}
  inline Node_gui* get_node_gui(){return node_gui;}
  inline Node_operation* get_node_ope(){return node_ope;}

  inline Capture* get_captureManager(){return captureManager;}
  inline Configuration* get_configManager(){return configManager;}
  inline Recorder* get_recordManager(){return recordManager;}
  inline HTTP* get_httpsManager(){return httpsManager;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;
  Node_load* node_load;
  Node_gui* node_gui;

  HTTP* httpsManager;
  PCAP_reader* pcapManager;
  Recorder* recordManager;
  Capture* captureManager;
  Configuration* configManager;
};

#endif
