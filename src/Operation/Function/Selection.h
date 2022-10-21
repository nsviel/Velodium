#ifndef Selection_H
#define Selection_H

class Node_operation;
class Scene;
class Object;
class Transforms;
class Attribut;
class Camera;
class Dimension;
class CoordTransform;
class Renderer;

#include "../../common.h"

class Selection
{
public:
  //Constructor / Destructor
  Selection(Node_operation* node_ope);
  ~Selection();

public:
  //Drawing functions
  void update();
  void validate();

  //Point selection
  void selectionPoint(vec3 point);
  void mark_pointCreation(vec3 point);
  bool mark_pointSupression(vec3 point);
  void mark_pointColor(Cloud* sphere, int num);
  void mark_pointLocation();
  void mark_supressAll();
  void mark_supressSelectedPoints_all();
  void mark_supressSelectedPoints(Cloud* cloud);

  //Mouse interactivity
  void mouse_selection(int selectMouseMode);
  void mouse_frameSelection(vec2 pt1, vec2 pt2);
  void mouse_drawFrame(vec2 point1, vec2 point2);
  void mouse_cloudPicking();
  vec3 mouse_cameraPt();
  vec3 mouse_click_point();
  void update_glDims();

  //Plane
  void mark_planeCreation();
  void mark_planeABpoints(Cloud* cloud);
  void mark_planeLocation();

  //Setters / Getters
  inline void set_markMode(string mode){this->markMode = mode;}
  inline void set_selectionSensibility(float value){this->selectSensibility = value;}
  inline void set_guiX(float value){this->gui_X = value;}
  inline void set_guiY(float value){this->gui_Y = value;}
  inline void set_glX(float value){this->gl_X = value;}
  inline void set_glY(float value){this->gl_Y = value;}

private:
  Dimension* dimManager;
  Scene* sceneManager;
  Attribut* attribManager;
  Transforms* transformManager;
  Object* objectManager;
  Camera* cameraManager;
  CoordTransform* coordManager;
  Renderer* renderManager;

  list<Cloud*> list_Mark;
  list<int> list_glyph;
  Cloud* planeMark;
  Cloud* Spectralon;
  vec3 A, B, C, D, E, F;
  string markMode;
  float selectSensibility;
  float angle;
  float gui_X, gui_Y, gl_X, gl_Y;
  int nbMark;
  int ID_plane;
};

#endif
