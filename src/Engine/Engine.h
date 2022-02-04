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
class Camera;
class Dimension;
class GUI;
class Shader;
class Renderer;
class CT_ICP;
class Configuration;
class Module;
class Operation_node;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Dimension* dim, Camera* control, Shader* shader, Renderer* render, Configuration* config);
  ~Engine();

public:
  //Program functions
  void init_database();
  void loop();
  void Exit();
  void reset();

  //Subfunctions
  void draw_clouds();

  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Camera* get_CameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline CT_ICP* get_cticpManager(){return cticpManager;}
  inline Configuration* get_configManager(){return configManager;}
  inline Module* get_moduleManager(){return moduleManager;}
  inline Operation_node* get_node_opeManager(){return node_opeManager;}

private:
  Dimension* dimManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  GUI* guiManager;
  Shader* shaderManager;
  Renderer* renderManager;
  CT_ICP* cticpManager;
  Configuration* configManager;
  Module* moduleManager;
  Operation_node* node_opeManager;

  uint modelID, comID;
};

#endif
