#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

#include "../../Operation/Transformation/Transformation.h"

#include "../../common.h"

class Scene;
class Camera;
class Attribut;
class Pather;
class Transformation;
class Extraction;
class Selection;
class Heatmap;
class Dimension;
class Node_gui;
class Boxing;
class Player;
class Pose;
class Graph;
class Light;


class GUI_Control
{
public:
  //Constructor / Destructor
  GUI_Control(Node_gui* node_gui);
  ~GUI_Control();

public:
  //Main function
  void make_control();

  //Mouse function
  void control_frame_selection();
  void control_mouse();
  void control_mouse_wheel();

  //Keyboard function
  void control_keyboard_oneAction();
  void control_keyboard_translation();
  void control_keyboard_camMove();
  void control_keyboard_ctrlAction();

  //Specific function
  void key_suppr();
  void key_c();
  void key_translation(vec3 trans);
  void key_rotation(vec3 rotat);

  inline float* get_transCoef(){return &cloud_trans_speed;}
  inline float* get_rotatDegree(){return &cloud_rotat_degree;}

private:
  Transformation* transformManager;
  Pose* poseManager;
  Node_gui* node_gui;
  Dimension* dimManager;
  Camera* cameraManager;
  Scene* sceneManager;
  Attribut* attribManager;
  Pather* pathManager;
  Extraction* extractionManager;
  Selection* selectionManager;
  Heatmap* heatmapManager;
  Player* playerManager;
  Boxing* boxingManager;
  Graph* graphManager;
  Light* lightManager;

  float cloud_trans_speed;
  float cloud_rotat_degree;
};

#endif
