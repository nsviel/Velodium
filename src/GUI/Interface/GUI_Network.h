#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

#include "../../common.h"

class Node_gui;
class HTTP;
class PCAP_reader;


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network(Node_gui* node_gui);
  ~GUI_Network();

public:
  void design_Network();

  //PCAP stuff
  void pcap_connection();

  //HTTP
  void http_connection();
  void http_parameter();

private:
  PCAP_reader* pcapManager;
  HTTP* httpsManager;

  int item_width;
};

#endif
