#ifndef GRAPH_H
#define GRAPH_H

#include "../../common.h"

class GPU_transfert;
class Node_scene;
class Scene;


class Graph
{
public:
  //Constructor / Destructor
  Graph(Node_scene* node);
  ~Graph();

public:
  //Subfunctions
  void update_cloud_oID(list<Cloud*>* list);
  void selection_setNext();
  void selection_setCloud(int ID);
  void selection_setSubset(Cloud* cloud, int ID);
  void selection_cloudByName(string name);
  void selection_setCloud(Cloud* cloud);

private:
  Scene* sceneManager;
};

#endif
