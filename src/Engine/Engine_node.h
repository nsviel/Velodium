#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "../common.h"

class Operation_node;
class Load_node;
class Module_node;
class GUI_node;
class Configuration;
class Scene;
class Glyphs;
class Camera;
class Shader;
class Renderer;
class Viewport;
class Dimension;
class Engine;
class CoreGLengine;

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Engine_node
{
public:
  //Constructor / Destructor
  Engine_node(CoreGLengine* ogl);
  ~Engine_node();

public:
  //Main functions
  void update();
  void runtime();
  void reset();
  void exit();

  inline Module_node* get_node_module(){return node_module;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline GUI_node* get_node_gui(){return node_gui;}
  inline Load_node* get_node_load(){return node_load;}

  inline Configuration* get_configManager(){return configManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Scene* get_sceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Camera* get_cameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline Viewport* get_viewportManager(){return viewportManager;}
  inline Engine* get_engineManager(){return engineManager;}
  inline CoreGLengine* get_glManager(){return glManager;}

private:
  Operation_node* node_ope;
  Load_node* node_load;
  Module_node* node_module;
  GUI_node* node_gui;

  CoreGLengine* glManager;
  Configuration* configManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  Shader* shaderManager;
  Renderer* renderManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Engine* engineManager;
};

#endif
