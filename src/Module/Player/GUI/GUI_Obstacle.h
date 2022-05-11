#ifndef GUI_OBSTACLE_H
#define GUI_OBSTACLE_H

#include "../../../common.h"

class GUI_Interface;
class GUI_module;

class Module_node;
class Scene;
class Obstacle;
class CoordTransform;
class Player_node;
class Configuration;
class Color;


class GUI_Obstacle
{
public:
  //Constructor / Destructor
  GUI_Obstacle(GUI_module* gui_module_module);
  ~GUI_Obstacle();

public:
  //Main function
  void design_Obstacle();
  void runtime_display_naming();

  //Subfunctions
  void compute_draw_text(string text, vec3 position);
  void state_configuration();
  void state_online();
  void parameter_online();
  void parameter_interfacing();

private:
  GUI_Interface* gui_interface;
  GUI_module* gui_module;

  Module_node* node_module;
  Player_node* node_player;
  Obstacle* obstacleManager;
  CoordTransform* coordManager;
  Scene* sceneManager;
  Configuration* configManager;
  Color* colorManager;

  int label_ID;
  int item_width;
  string screenshot_path;
};

#endif
