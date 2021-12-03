#ifndef GUI_SENDER_H
#define GUI_SENDER_H

class Sender;

#include "../../common.h"


class GUI_Sender
{
public:
  //Constructor / Destructor
  GUI_Sender();
  ~GUI_Sender();

public:
  void design_Sender();

private:
  Sender* senderManager;

};

#endif
