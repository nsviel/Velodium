#ifndef MOD_OPERATION_H
#define MOD_OPERATION_H

#include "../../../common.h"

class Scene;
class Glyphs;
class Filter;
class Node_operation;
class Pose;
class Fitting;
class Extraction;
class Selection;


class MOD_operation
{
public:
  //Constructor / Destructor
  MOD_operation(Node_operation* node_ope);
  ~MOD_operation();

public:
  void window_selection();
  void window_fitting();

private:
  Pose* poseManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  Filter* filterManager;
  Fitting* fitManager;
  Extraction* extractionManager;
  Selection* selectionManager;

  int item_width;
};

#endif
