#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Scene;
class GUI_node;
class Obstacle;
class Scenario;
class Interfacing;
class CoordTransform;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(GUI_node* node_gui);
  ~GUI_Obstacle();

public:
  //Main function
  void design_Obstacle();

  //Subfunctions
  void compute_scenario();
  void compute_obstacle();
  void element_state();
  void parameter();
  void parameter_interfacing();

  //Obstacle visual naming
  void make_obstacleName(Subset* subset);
  void drawText(string text, vec3 position);

private:
  Obstacle* obstacleManager;
  Interfacing* ioManager;
  CoordTransform* coordManager;
  Scene* sceneManager;
  Scenario* scenarioManager;
  GUI_node* node_gui;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
