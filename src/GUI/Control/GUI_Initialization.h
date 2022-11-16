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

  void operation_init(int mode);
  void operation_cloud(Cloud* cloud);
  void operation_option();

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Pather* pathManager;

  bool remove_cloud;
  int cloud_scale;
  string lidar_model;
};

#endif
