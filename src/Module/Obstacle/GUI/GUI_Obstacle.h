#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Module_obstacle;
class Obstacle;
class Prediction;
class CoordTransform;
class HTTP;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(Module_obstacle* module);
  ~GUI_Obstacle();

public:
  //Main function
  void design_obstacle();
  void design_state();
  void runtime_display_naming(Cloud* cloud);

  //Subfunctions
  void compute_draw_text(string text, vec3 position);
  void parameter_online();
  void parameter_interfacing();

private:
  Obstacle* obstacleManager;
  CoordTransform* coordManager;
  Prediction* predManager;
  HTTP* httpsManager;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
