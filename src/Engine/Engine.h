#ifndef ENGINE_H
#define ENGINE_H

#include "../common.h"

class Scene;
class Glyphs;

class Configuration_node;
class Module_node;
class Operation_node;
class Engine_node;
class GUI_node;

using namespace std;
using namespace glm;


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
  void exit();
  void reset();
  void runtime();

  //Subfunctions
  void draw_clouds();

  inline Configuration_node* get_node_config(){return node_config;}
  inline Module_node* get_node_module(){return node_module;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Engine_node* get_node_engine(){return node_engine;}
  inline GUI_node* get_node_gui(){return node_gui;}

private:
  Scene* sceneManager;
  Glyphs* glyphManager;

  Configuration_node* node_config;
  Module_node* node_module;
  Operation_node* node_ope;
  Engine_node* node_engine;
  GUI_node* node_gui;

  uint modelID, comID;
};

#endif
