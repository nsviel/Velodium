#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Camera.h"

#include "../../../common.h"

class Dimension;

class Viewport
{
public:
  //Constructor / Destructor
  Viewport(Dimension* dimension);
  ~Viewport();

public:
  void update_viewport();
  void update_viewport(int loop_cpt);

  inline int get_number_viewport(){return nb_viewport;}

private:
  Dimension* dimManager;
  Camera* cameraManager;

  vector<int> viewport_vec;
  vec2 pos, dim;
  int nb_viewport;
};

#endif
