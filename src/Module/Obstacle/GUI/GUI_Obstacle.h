#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

class Scene;
class Obstacle;
class CoordTransform;

#include "../../../Engine/Engine.h"
#include "../../../Operation/Functions/CoordTransform.h"
#include "../../../common.h"


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(Engine* engineManager);
  ~GUI_Obstacle();

public:
  void design_Obstacle();

  void run();
  void make_obstacleName(Subset* subset);

  void drawText(string text, vec3 position);

private:
  Obstacle* obstacleManager;
  CoordTransform* coordManager;
  Scene* sceneManager;

  int label_ID;
};

#endif
