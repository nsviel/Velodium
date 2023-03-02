#ifndef GRAPH_H
#define GRAPH_H

#include "../../common.h"

class GPU_transfert;
class Node_scene;


class Graph
{
public:
  //Constructor / Destructor
  Graph(Node_scene* node);
  ~Graph();

public:
  void insert_loaded_cloud(Cloud* cloud);
  void draw_all();



};

#endif
