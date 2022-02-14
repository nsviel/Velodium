#ifndef WIN_OPERATION_H
#define WIN_OPERATION_H

#include "../../common.h"

class Scene;
class Glyphs;
class Filter;
class Operation_node;
class Transforms;
class Fitting;
class Extraction;
class Selection;


class WIN_operation
{
public:
  //Constructor / Destructor
  WIN_operation(Operation_node* node_ope);
  ~WIN_operation();

public:
  void window_filter();
  void window_selection();
  void window_transformation();
  void window_fitting();
  void window_extractCloud();
  void window_cutCloud();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Filter* filterManager;
  Transforms* transformManager;
  Fitting* fitManager;
  Extraction* extractionManager;
  Selection* selectionManager;

  int item_width;
};

#endif
