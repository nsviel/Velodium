#ifndef ENGINE_H
#define ENGINE_H

#include "../../Scene/Data/Data.h"
#include "../../common.h"

class Node_engine;
class Scene;
class Glyphs;
class Object;
class Camera;
class Texture;
class GPU_data;


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

  //Drawing function
  void draw_light();
  void draw_untextured_cloud();
  void draw_textured_cloud();

private:
  Node_engine* node_engine;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Object* objectManager;
  Camera* cameraManager;
  Texture* texManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
