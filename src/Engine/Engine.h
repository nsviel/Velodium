#ifndef ENGINE_H
#define ENGINE_H

/**
 * \file Engine.h
 * \brief Data processing unit
 *
 * Take responsability of class objects management and drawing stuff
 *
 */

#include "../common.h"

using namespace std;
using namespace glm;

class Scene;
class Glyphs;

class Configuration_node;
class Module_node;
class Operation_node;
class Engine_node;

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
  void exit();
  void reset();
  void runtime();

  //Subfunctions
  void draw_clouds();

  inline Configuration_node* get_node_configManager(){return node_configManager;}
  inline Module_node* get_node_moduleManager(){return node_moduleManager;}
  inline Operation_node* get_node_opeManager(){return node_opeManager;}
  inline Engine_node* get_node_engineManager(){return node_engineManager;}

private:
  Scene* sceneManager;
  Glyphs* glyphManager;

  Configuration_node* node_configManager;
  Module_node* node_moduleManager;
  Operation_node* node_opeManager;
  Engine_node* node_engineManager;

  uint modelID, comID;
};

#endif
