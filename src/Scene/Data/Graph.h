#ifndef GRAPH_H
#define GRAPH_H

#include "../../common.h"

class GPU_data;
class Node_scene;
class Scene;


class Graph
{
public:
  //Constructor / Destructor
  Graph(Node_scene* node);
  ~Graph();

public:
  void selection_setNext();
  void set_selected_collection(int ID);
  void set_selected_object(Collection* collection, int ID);
  void selection_cloudByName(string name);
  void set_selected_collection(Collection* collection);
  void object_clicked(Collection* collection, int ID);

private:
  Scene* sceneManager;
};

#endif
