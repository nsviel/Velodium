#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "../common.h"

class Node_operation;
class Node_load;
class Node_module;
class Node_gui;
class Node_interface;

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
class Object;
class Followup;


class Node_engine
{
public:
  //Constructor / Destructor
  Node_engine(CoreGLengine* ogl);
  ~Node_engine();

public:
  //Main functions
  void update();
  void runtime();
  void reset();
  void exit();

  inline Node_module* get_node_module(){return node_module;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_gui* get_node_gui(){return node_gui;}
  inline Node_load* get_node_load(){return node_load;}
  inline Node_interface* get_node_interface(){return node_interface;}

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
  inline Object* get_objectManager(){return objectManager;}
  inline Followup* get_followManager(){return followManager;}

private:
  Node_operation* node_ope;
  Node_load* node_load;
  Node_module* node_module;
  Node_interface* node_interface;
  Node_gui* node_gui;

  Followup* followManager;
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
  Object* objectManager;
};

#endif
