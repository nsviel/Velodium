#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

class Network;

#include "../../common.h"


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network();
  ~GUI_Network();

public:
  void design_Network();

private:
  Network* netManager;

};

#endif
