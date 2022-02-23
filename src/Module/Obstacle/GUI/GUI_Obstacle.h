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

  //Actions
  void compute_scenario();
  void compute_display_naming();
  void compute_draw_text(string text, vec3 position);

  //Watchers
  void state_watcher();
  void state_online();

  //Parameters
  void parameter_watcher();
  void parameter_online();
  void parameter_interfacing();

  //Watchers
  void watcher_AI_pred();
  void watcher_gps();

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
