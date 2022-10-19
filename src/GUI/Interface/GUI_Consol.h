#ifndef GUI_CONSOL_H
#define GUI_CONSOL_H

class Dimension;
class Node_gui;
class Configuration;

#include "../../common.h"


class GUI_consol
{
public:
  //Constructor / Destructor
  GUI_consol(Node_gui* node_gui);
  ~GUI_consol();

public:
  //Main function
  void design_consol();
  void draw_consol();
  void update_dimension();

  inline vec2 get_bp_dim(){return dim_bp;}

private:
  Configuration* node_config;
  Dimension* dimManager;

  vec2 dim_bp;
};

#endif
