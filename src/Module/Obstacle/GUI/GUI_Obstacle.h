#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Module_obstacle;
class Obstacle;
class GUI_Network;
class Prediction;
class CoordTransform;
class HTTP;
class Scene;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(Module_obstacle* module);
  ~GUI_Obstacle();

public:
  //Main function
  void design_obstacle();
  void design_prediction();

  //Draw label text function
  void runtime_display_naming();
  void compute_draw_text(string text, vec3 position);

  //Parameter function
  void parameter_prediction();

  //State function
  void state_prediction();

private:
  Obstacle* obstacleManager;
  CoordTransform* coordManager;
  Prediction* predManager;
  HTTP* httpsManager;
  Scene* sceneManager;
  GUI_Network* gui_network;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
