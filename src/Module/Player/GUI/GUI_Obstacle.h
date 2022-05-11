#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class GUI_module;
class Scene;
class Obstacle;
class CoordTransform;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(GUI_module* gui_module);
  ~GUI_Obstacle();

public:
  //Main function
  void design_obstacle();
  void runtime_display_naming();

  //Subfunctions
  void compute_draw_text(string text, vec3 position);
  void parameter_online();
  void parameter_interfacing();

private:
  GUI_module* gui_module;

  Obstacle* obstacleManager;
  CoordTransform* coordManager;
  Scene* sceneManager;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
