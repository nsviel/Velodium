#ifndef GUI_FILEMANAGER_H
#define GUI_FILEMANAGER_H

#include "../../common.h"

class Scene;
class Dimension;
class GUI_Modal;
class Node_gui;

class GUI_fileManager
{
public:
  //Constructor / Destructor
  GUI_fileManager(Node_gui* node_gui);
  ~GUI_fileManager();

public:
  void fileManager();
  void cloudManager(Cloud* cloud);

  void info_cloud(Cloud* cloud);
  void info_subset(Subset* subset);
  void info_iconAction(Cloud* cloud);

private:
  Scene* sceneManager;
  Dimension* dimManager;
  GUI_Modal* gui_window;
};

#endif
