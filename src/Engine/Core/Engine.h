#ifndef ENGINE_H
#define ENGINE_H

#include "../../common.h"

class Node_engine;
class Scene;
class Glyphs;
class GUI;
class Object;
class Camera;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Node_engine* engine);
  ~Engine();

public:
  //Program functions
  void runtime_scene();
  void runtime_draw_glyph();
  void runtime_draw_cloud();
  void runtime_camera();

  //Subfunctions
  void draw_mesh(Subset* subset, string draw_type);

private:
  Node_engine* node_engine;
  Scene* sceneManager;
  Glyphs* glyphManager;
  GUI* guiManager;
  Object* objectManager;
  Camera* cameraManager;

  bool is_visualization;
  uint modelID, comID;
};

#endif
