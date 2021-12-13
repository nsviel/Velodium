#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

class Obstacle;

#include "../../common.h"


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle();
  ~GUI_Obstacle();

public:
  void design_Obstacle();

private:
  Obstacle* obstacleManager;

};

#endif
