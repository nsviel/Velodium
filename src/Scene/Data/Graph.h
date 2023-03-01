#ifndef GRAPH_H
#define GRAPH_H

#include "../../common.h"

class Node_scene;
class Object_base;
class Cloud_base;
class Mesh_base;

class Graph
{
public:
  //Constructor / Destructor
  Graph(Node_scene* node);
  ~Graph();

public:

private:
  list<Object_base*>* list_object;
  list<Cloud_base*>* list_cloud;
  list<Mesh_base*>* list_mesh;

};

#endif
