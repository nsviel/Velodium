#ifndef ENGINE_H
#define ENGINE_H

#include "../common.h"

class Scene;
class Glyphs;
class GUI;
class Object;

class Node_engine;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Node_engine* engine);
  ~Engine();

public:
  //Program functions
  void runtime_scene();

  //Subfunctions
  void runtime_draw_glyph();
  void runtime_draw_cloud();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  GUI* guiManager;
  Object* objectManager;

  Node_engine* node_engine;

  bool is_visualization;
  uint modelID, comID;
};

#endif
