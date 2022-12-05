#ifndef VIEWPORT_H
#define VIEWPORT_H

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
  void viewport_update(int ID);
  void viewport_reset();

  void update_viewport();
  void update_viewport(int loop_cpt);

  inline Viewport_obj* get_viewport_main(){return &view_main;}
  inline int get_number_viewport(){return nb_viewport;}

private:
  Dimension* dimManager;
  Configuration* configManager;

  Viewport_obj view_main;
  Viewport_obj view_map;

  vector<int> viewport_vec;
  vec2 pos, dim;
  int nb_viewport;
};

#endif
