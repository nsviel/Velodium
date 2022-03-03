#ifndef ENGINE_H
#define ENGINE_H

#include "../common.h"

class Scene;
class Glyphs;
class GUI;

class Engine_node;
class GUI_node;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Engine_node* engine);
  ~Engine();

public:
  //Program functions
  void loop_scene();
  void loop_gui();

  //Subfunctions
  void runtime_draw_clouds();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline GUI_node* get_node_gui(){return node_gui;}

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  GUI* guiManager;

  Engine_node* node_engine;
  GUI_node* node_gui;

  bool is_visualization;
  uint modelID, comID;
};

#endif
