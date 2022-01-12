#ifndef ENGINE_H
#define ENGINE_H

/**
 * \file Engine.h
 * \brief Data processing unit
 *
 * Take responsability of class objects management and drawing stuff
 *
 */

class Scene;
class Glyphs;
class Camera;
class Dimension;
class GUI;
class Heatmap;
class Shader;

#include "../common.h"

using namespace std;
using namespace glm;


class Engine
{
public:
  //Constructor / Destructor
  Engine(Dimension* dim, Camera* control, Shader* shader);
  ~Engine();

public:
  //Program functions
  void init_database();
  void loop();
  void Exit();

  //Subfunctions
  void draw_clouds();

  inline Scene* get_SceneManager(){return sceneManager;}
  inline Glyphs* get_glyphManager(){return glyphManager;}
  inline Dimension* get_dimManager(){return dimManager;}
  inline Camera* get_CameraManager(){return cameraManager;}
  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Shader* get_shaderManager(){return shaderManager;}

private:
  Dimension* dimManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  GUI* guiManager;
  Heatmap* heatmapManager;
  Shader* shaderManager;

  uint modelID, comID;
};

#endif
