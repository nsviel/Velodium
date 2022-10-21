#ifndef GUI_INITIALIZATION_H
#define GUI_INITIALIZATION_H

#include "../../common.h"

class Node_gui;
class Scene;
class Loader;
class Pather;


class GUI_Initialization
{
public:
  //Constructor / Destructor
  GUI_Initialization(Node_gui* node_gui);
  ~GUI_Initialization();

public:
  void init_gui();
  void init_mode(int mode);

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Pather* pathManager;
};

#endif
