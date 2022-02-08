#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

#include "../../Operation/Transformation/Transforms.h"

#include "../../common.h"

class Scene;
class Camera;
class Attribut;
class Pather;
class Transforms;
class Extraction;
class Glyphs;
class Selection;
class Heatmap;
class Dimension;
class GUI_node;


class GUI_control
{
public:
  //Constructor / Destructor
  GUI_control(GUI_node* node_gui);
  ~GUI_control();

public:
  //Main function
  void make_control();

  //Subfunctions
  void control_frameSelection();

  void control_mouse();
  void control_mouse_wheel();

  void control_keyboard_oneAction();
  void control_keyboard_translation();
  void control_keyboard_camMove();
  void control_keyboard_ctrlAction();

  inline float* get_transCoef(){return &cloud_trans_speed;}
  inline float* get_rotatDegree(){return &cloud_rotat_degree;}
  inline int* get_mouseWheelMode(){return &wheel_mode;}

private:
  Transforms transformManager;

  GUI_node* node_gui;
  Dimension* dimManager;
  Camera* cameraManager;
  Scene* sceneManager;
  Attribut* attribManager;
  Pather* pathManager;
  Extraction* extractionManager;
  Glyphs* glyphManager;
  Selection* selectionManager;
  Heatmap* heatmapManager;

  float cloud_trans_speed;
  float cloud_rotat_degree;
  int wheel_mode;
};

#endif
