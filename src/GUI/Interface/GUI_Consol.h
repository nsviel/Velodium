#ifndef GUI_CONSOL_H
#define GUI_CONSOL_H

class Engine;
class Dimension;

#include "../../common.h"

class GUI_consol
{
public:
  //Constructor / Destructor
  GUI_consol(Engine* renderer);
  ~GUI_consol();

public:
  //Main function
  void design_consol();

  inline float* get_panel_Y(){return &panel_Y;}
  inline float* get_panel_X(){return &panel_X;}
  inline void set_panel_X(float value){this->panel_X = value;}

private:
  Engine* engineManager;
  Dimension* dimManager;

  float panel_X;
  float panel_Y;
};

#endif
