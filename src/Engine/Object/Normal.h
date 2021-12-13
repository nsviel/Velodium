#ifndef NORMAL_H
#define NORMAL_H

#include "../../common.h"


class Normal
{
public:
  //Constructor / Destructor
  Normal();
  ~Normal();

public:
  void obj_normal(Subset* subset);
  void update_normal(Subset* subset);

private:
  vec4 normal_color;
  int normal_size;
};

#endif