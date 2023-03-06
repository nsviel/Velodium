#ifndef ENGINE_H
#define ENGINE_H

#include "../../common.h"

class Node_engine;
class Scene;
class Glyphs;
class GUI;
class Object;
class Camera;
class Texture;
class GPU_transfert;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Node_engine* engine);
  ~Engine();

public:
  //Misc drawing function
  void draw_untextured_glyph();
  void arcball_cam_lookat();

  //Cloud drawing function
  void draw_untextured_cloud();
  void draw_textured_cloud();

  //Subset drawing function
  void draw_textured_subset(Subset* subset);
  void draw_untextured_subset(Subset* subset);

private:
  Node_engine* node_engine;
  Scene* sceneManager;
  Glyphs* glyphManager;
  GUI* guiManager;
  Object* objectManager;
  Camera* cameraManager;
  Texture* texManager;
  GPU_transfert* gpuManager;
};

#endif
