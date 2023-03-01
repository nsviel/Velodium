#ifndef SLAM_OBJECT_NORMAL_H
#define SLAM_OBJECT_NORMAL_H

#include "../Base/SLAM_object_base.h"


class Normal : public SLAM_object_base
{
public:
  //Constructor / Destructor
  Normal();
  ~Normal();

public:
  void create_glyph(Subset* subset);
  void update_normal_subset(Subset* subset);
  void update_normal_subset(Subset* subset, vector<vec3>& xyz_s, vector<vec3>& Nxyz_s);

  inline bool* get_visibility(){return &visibility;}
  inline int* get_size(){return &size;}

private:
  bool visibility;
  vec4 color;
  int size;
  int width;
};

#endif
