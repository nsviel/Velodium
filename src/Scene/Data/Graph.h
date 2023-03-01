#ifndef GRAPH_H
#define GRAPH_H

#include "../../common.h"

class Node_scene;
class Mesh_base;
class Cloud_base;


class Graph
{
public:
  //Constructor / Destructor
  Graph(Node_scene* node);
  ~Graph();

public:

private:
  list<Mesh_base*>* list_mesh;
  list<Cloud_base*>* list_cloud;
};

#endif
