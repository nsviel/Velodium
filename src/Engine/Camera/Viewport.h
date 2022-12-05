#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Camera.h"
#include "struct_viewport.h"

#include "../../common.h"

class Dimension;
class Configuration;


class Viewport
{
public:
  //Constructor / Destructor
  Viewport(Dimension* dimension);
  ~Viewport();

public:
  void viewport_init();
  void viewport_init_main();
  void viewport_init_map();
  void update_viewport();
  void update_viewport(int loop_cpt);

  inline int get_number_viewport(){return nb_viewport;}

private:
  Dimension* dimManager;
  Camera* cameraManager;
  Configuration* configManager;

  Viewport_obj view_main;
  Viewport_obj view_map;
  Viewport_obj* viewport;

  vector<int> viewport_vec;
  vec2 pos, dim;
  int nb_viewport;
};

#endif
