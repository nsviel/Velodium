#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Scene;
class GUI_node;
class Obstacle;
class Obstacle_IO;
class CoordTransform;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(GUI_node* node_gui);
  ~GUI_Obstacle();

public:
  void design_Obstacle();

  void compute_obstacle();
  void make_obstacleName(Subset* subset);
  void drawText(string text, vec3 position);
  void parameters();

private:
  Obstacle* obstacleManager;
  Obstacle_IO* ioManager;
  CoordTransform* coordManager;
  Scene* sceneManager;

  int label_ID;
  string screenshot_path;
};

#endif
