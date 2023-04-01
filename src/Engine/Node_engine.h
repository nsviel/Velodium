#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include "../Specific/Base/Node.h"

class Node_operation;
class Node_load;
class Node_module;
class Node_gui;
class Node_scene;
class Node_interface;

class Configuration;
class Camera;
class Shader;
class Renderer;
class GPU_screenshot;
class GPU_fbo;
class GPU_render;
class Viewport;
class Engine;
class CoreGLengine;
class Followup;
class Texture;
class Light;
class Dimension;


class Node_engine : public Node
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
  inline Node_scene* get_node_scene(){return node_scene;}
  inline Node_interface* get_node_interface(){return node_interface;}

  inline Configuration* get_configManager(){return configManager;}
  inline Camera* get_cameraManager(){return cameraManager;}
  inline Shader* get_shaderManager(){return shaderManager;}
  inline Renderer* get_renderManager(){return renderManager;}
  inline GPU_fbo* get_gpu_fbo(){return gpu_fbo;}
  inline Viewport* get_viewportManager(){return viewportManager;}
  inline Engine* get_engineManager(){return engineManager;}
  inline CoreGLengine* get_glManager(){return glManager;}
  inline Followup* get_followManager(){return followManager;}
  inline Texture* get_texManager(){return texManager;}
  inline Light* get_lightManager(){return lightManager;}
  inline GPU_screenshot* get_gpu_screenshot(){return gpu_screenshot;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline GPU_render* get_gpu_render(){return gpu_render;}

private:
  Node_operation* node_ope;
  Node_load* node_load;
  Node_module* node_module;
  Node_interface* node_interface;
  Node_scene* node_scene;
  Node_gui* node_gui;

  Texture* texManager;
  Followup* followManager;
  CoreGLengine* glManager;
  Configuration* configManager;
  Camera* cameraManager;
  Shader* shaderManager;
  GPU_screenshot* gpu_screenshot;
  Renderer* renderManager;
  GPU_fbo* gpu_fbo;
  GPU_render* gpu_render;
  Dimension* dimManager;
  Viewport* viewportManager;
  Engine* engineManager;
  Light* lightManager;
};

#endif
