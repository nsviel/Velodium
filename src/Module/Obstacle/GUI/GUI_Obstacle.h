#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Module_obstacle;
class Obstacle;
class Prediction;
class Coordinate;
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
  Coordinate* coordManager;
  Prediction* predManager;
  HTTP* httpsManager;
  Scene* sceneManager;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
