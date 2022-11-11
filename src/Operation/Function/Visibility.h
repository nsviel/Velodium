#ifndef VISIBILITY_H
#define VISIBILITY_H

#include "../../common.h"

class Node_operation;
class Scene;
class Object;
class Glyphs;
class Attribut;


class Visibility
{
public:
  //Constructor / Destructor
  Visibility(Node_operation* node_ope);
  ~Visibility();

  void compute_box_MinMax(Cloud* cloud, vec3 min_perc, vec3 max_perc);
  void compute_visibility(Cloud* cloud);
  void compute_visibility(Cloud* cloud, int ID);

  void supress_selected_point(Cloud* cloud);
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
