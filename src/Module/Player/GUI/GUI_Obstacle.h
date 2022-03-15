#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class Scene;
class GUI_module;
class Obstacle;
class Interface_node;
class CoordTransform;
class Player_node;
class Configuration;
class Color;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(GUI_module* node_gui_module);
  ~GUI_Obstacle();

public:
  //Main function
  void design_Obstacle();
  void runtime_display_naming();

  //Actions
  void state_configuration();
  void compute_draw_text(string text, vec3 position);

  //Watchers
  void state_watcher();
  void state_online();

  //Parameters
  void parameter_online();
  void parameter_interfacing();

private:
  Player_node* node_player;
  Obstacle* obstacleManager;
  Interface_node* node_interface;
  CoordTransform* coordManager;
  Scene* sceneManager;
  Configuration* configManager;
  GUI_module* node_gui;
  Color* colorManager;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
