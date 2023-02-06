#ifndef MOD_BOXING_H
#define MOD_BOXING_H

#include "../../../common.h"

class Scene;
class Glyphs;
class Node_operation;
class Extraction;
class Object;
class Boxing;


class MOD_boxing
{
public:
  //Constructor / Destructor
  MOD_boxing(Node_operation* node_ope);
  ~MOD_boxing();

public:
  void box_shape();
  void design_boxing();

  //Specific function
  void control_box();
  void change_box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Object* objectManager;
  Extraction* extractionManager;
  Boxing* boxingManager;

  int item_width;
};

#endif
