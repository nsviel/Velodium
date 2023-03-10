#ifndef NODE_SCENE_H
#define NODE_SCENE_H

#include "../Specific/Base/Node.h"

class Node_engine;
class Scene;
class Glyphs;
class Object;
class Graph;


class Node_scene : public Node
{
public:
  //Constructor / Destructor
  Node_scene(Node_engine* node);
  ~Node_scene();

public:
  //Main functions
  void init();
  void update();
  void reset();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Scene* get_sceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Object* get_objectManager(){return objectManager;}
  inline Graph* get_graphManager(){return graphManager;}

private:
  Node_engine* node_engine;

  Scene* sceneManager;
  Glyphs* glyphManager;
  Object* objectManager;
  Graph* graphManager;
};

#endif
