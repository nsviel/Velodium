#ifndef GUI_FILEMANAGER_H
#define GUI_FILEMANAGER_H

#include "../../common.h"

class Scene;
class Dimension;
class GUI_Modal;
class Node_gui;
class Graph;


class GUI_fileManager
{
public:
  //Constructor / Destructor
  GUI_fileManager(Node_gui* node_gui);
  ~GUI_fileManager();

public:
  void fileManager();
  void cloudManager(Collection* collection);

  void info_collection(Collection* collection);
  void info_subset(Cloud* cloud);
  void info_iconAction(Collection* collection);


  void treeview();

private:
  Scene* sceneManager;
  Dimension* dimManager;
  GUI_Modal* gui_window;
  Graph* graphManager;
};

#endif
