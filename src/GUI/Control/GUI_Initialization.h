#ifndef GUI_INITIALIZATION_H
#define GUI_INITIALIZATION_H

#include "../../common.h"

class GUI_node;
class Scene;
class Loader;
class Pather;
class Obstacle;
class Scenario;


class GUI_Initialization
{
public:
  //Constructor / Destructor
  GUI_Initialization(GUI_node* node_gui);
  ~GUI_Initialization();

public:
  void init_gui();
  void init_mode(int mode);

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Pather* pathManager;
  Obstacle* obstacleManager;
  Scenario* scenarioManager;
};

#endif
