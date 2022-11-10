#ifndef MOD_FILTER_H
#define MOD_FILTER_H

#include "../../../common.h"

class Scene;
class Filter;
class Node_operation;


class MOD_filter
{
public:
  //Constructor / Destructor
  MOD_filter(Node_operation* node_ope);
  ~MOD_filter();

public:
  void design_filter();

  //Specific function
  void filter_cylinder();
  void filter_byAngle();
  void filter_sphere();

private:
  Scene* sceneManager;
  Filter* filterManager;

  int item_width;
};

#endif
