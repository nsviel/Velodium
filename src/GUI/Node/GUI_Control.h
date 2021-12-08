#ifndef GUI_control_H
#define GUI_control_H

#include "../../Operation/Transformation/Transforms.h"

#include "../../common.h"

class Scene;
class Engine;
class Camera;
class Attribut;
class Operation;
class Transforms;
class Extraction;
class Glyphs;
class Selection;
class Heatmap;
class Dimension;


class GUI_control
{
public:
  //Constructor / Destructor
  GUI_control(Engine* renderer);
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

  inline float* get_transCoef(){return &move_trans_speed;}
  inline float* get_rotatDegree(){return &rotatDegree;}
  inline int* get_mouseWheelMode(){return &wheel_mode;}

private:
  Transforms transformManager;

  Dimension* dimManager;
  Engine* engineManager;
  Camera* cameraManager;
  Scene* sceneManager;
  Attribut* attribManager;
  Operation* opeManager;
  Extraction* extractionManager;
  Glyphs* glyphManager;
  Selection* selectionManager;
  Heatmap* heatmapManager;

  float transCoef;
  float move_trans_speed;
  float rotatDegree;
  int wheel_mode;
};

#endif
