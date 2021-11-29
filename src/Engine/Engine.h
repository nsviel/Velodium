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

#include "../common.h"

class Engine
{
public:
  //Constructor / Destructor
  Engine(Dimension* dim, Camera* control);
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

private:
  Dimension* dimManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Camera* cameraManager;
  GUI* guiManager;

  uint modelID, comID;
};

#endif
