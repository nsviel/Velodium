#ifndef WIN_FILTER_H
#define WIN_FILTER_H

#include "../../common.h"

class Scene;
class Filter;


class WIN_filter
{
public:
  //Constructor / Destructor
  WIN_filter(Filter* filter);
  ~WIN_filter();

public:
  void window_filter();

private:
  Scene* sceneManager;
  Filter* filterManager;

  int item_width;
};

#endif
