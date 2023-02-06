#ifndef OBJECT_CAR_H
#define OBJECT_CAR_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"

class Transformation;


class Car
{
public:
  //Constructor / Destructor
  Car();
  ~Car();

public:
  void create_car();
  void update_car_location(Cloud* cloud);
  void reset();

  inline Glyph* get_glyph(){return car;}
  inline bool* get_visibility(){return &visibility;}
  inline void set_visibility(bool value){this->visibility = value;}

private:
  Transformation* transformManager;
  Glyph* car;

  vec4 color;
  bool visibility;
  float lidar_height;
  int width;
};

#endif
