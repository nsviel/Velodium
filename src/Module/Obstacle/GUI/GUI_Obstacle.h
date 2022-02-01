#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Scene;
class Engine;
class Obstacle;
class Obstacle_IO;
class CoordTransform;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(Engine* engineManager);
  ~GUI_Obstacle();

public:
  void design_Obstacle();

  void compute_obstacle();
  void make_obstacleName(Subset* subset);
  void drawText(string text, vec3 position);

private:
  Obstacle* obstacleManager;
  Obstacle_IO* ioManager;
  CoordTransform* coordManager;
  Scene* sceneManager;

  int label_ID;
};

#endif
