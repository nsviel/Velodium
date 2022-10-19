#ifndef ENGINE_H
#define ENGINE_H

#include "../common.h"

class Scene;
class Glyphs;
class GUI;
class Object;

class Node_engine;
class Node_gui;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Node_engine* engine);
  ~Engine();

public:
  //Program functions
  void loop_scene();
  void loop_gui();

  //Subfunctions
  void runtime_draw_glyph();
  void runtime_draw_cloud();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_gui* get_node_gui(){return node_gui;}

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  GUI* guiManager;
  Object* objectManager;

  Node_engine* node_engine;
  Node_gui* node_gui;

  bool is_visualization;
  uint modelID, comID;
};

#endif
