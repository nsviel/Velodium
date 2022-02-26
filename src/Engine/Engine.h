#ifndef ENGINE_H
#define ENGINE_H

#include "../common.h"

class Scene;
class Glyphs;

class Module_node;
class Engine_node;
class GUI_node;

//Namespaces
using namespace std;
using namespace glm;
using namespace Eigen;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Engine_node* engine);
  ~Engine();

public:
  //Program functions
  void init_database();
  void loop_scene();
  void loop_gui();

  //Subfunctions
  void draw_clouds();

  inline Module_node* get_node_module(){return node_module;}
  inline Engine_node* get_node_engine(){return node_engine;}
  inline GUI_node* get_node_gui(){return node_gui;}

private:
  Scene* sceneManager;
  Glyphs* glyphManager;

  Module_node* node_module;
  Engine_node* node_engine;
  GUI_node* node_gui;

  bool with_window;
  uint modelID, comID;
};

#endif
