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
  void consol();
  void update_dimension();

  inline vec2 get_bp_dim(){return dim_bp;}

private:
  Engine* engineManager;
  Dimension* dimManager;

  vec2 dim_bp;
};

#endif
