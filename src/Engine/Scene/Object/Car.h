#ifndef OBJECT_CAR_H
#define OBJECT_CAR_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"
#include "../../Data/struct_frame.h"


class Car
{
public:
  //Constructor / Destructor
  Car();
  ~Car();

public:
  void create();
  void update(Cloud*cloud);
  void reset();

  inline Glyph* get_glyph(){return car;}

private:
  Glyph* car;
  vec4 color;
  int width;
};

#endif
