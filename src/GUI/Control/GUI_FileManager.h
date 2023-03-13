#ifndef GUI_FILEMANAGER_H
#define GUI_FILEMANAGER_H

#include "../../Scene/Data/Data.h"
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
  void data_tree();
  void collection_node(Collection* collection);

  void info_collection(Collection* collection);
  void info_iconAction(Collection* collection);
  void info_object(Object_* object);


  void treeview();

private:
  Scene* sceneManager;
  Dimension* dimManager;
  GUI_Modal* gui_window;
  Graph* graphManager;
  Data* data;
};

#endif
