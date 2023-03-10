#ifndef BOXING_H
#define BOXING_H

#include "../../common.h"

class Node_operation;
class Scene;
class Object;
class Glyphs;
class Attribut;


class Boxing
{
public:
  //Constructor / Destructor
  Boxing(Node_operation* node_ope);
  ~Boxing();

  void compute_box_MinMax(Collection* cloud, vec3 min_perc, vec3 max_perc);
  void compute_visibility(Collection* cloud);
  void compute_visibility(Collection* cloud, int ID);

  void supress_selected_point(Collection* cloud);
  void stop_boxing();

public:
  inline bool* get_highlightON(){return &highlightON;}

private:
  Scene* sceneManager;
  Object* objectManager;
  Glyphs* glyphManager;
  Attribut* attribManager;

  bool highlightON;
};

#endif
