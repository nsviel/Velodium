#ifndef OBJECT_NORMAL_H
#define OBJECT_NORMAL_H

#include "../Base/Glyph_source.h"


class Normal : public Glyph_source
{
public:
  //Constructor / Destructor
  Normal();
  ~Normal();

public:
  void create_glyph(Cloud* subset);
  void update_normal_cloud(Cloud* subset);
  void update_normal_cloud(Cloud* subset, vector<vec3>& xyz_s, vector<vec3>& Nxyz_s);

  inline bool* get_visibility(){return &visibility;}
  inline int* get_size(){return &size;}

private:
  bool visibility;
  vec4 color;
  int size;
  int width;
};

#endif
