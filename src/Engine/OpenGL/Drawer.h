#ifndef DRAWER_H
#define DRAWER_H

#include "../../common.h"

class Node_engine;
class Texture;


class Drawer
{
public:
  //Constructor / Destructor
  Drawer(Node_engine* engine);
  ~Drawer();

public:
  void draw_mesh(Subset* subset, string draw_type);

private:
  Node_engine* node_engine;
  Texture* texManager;
};

#endif
