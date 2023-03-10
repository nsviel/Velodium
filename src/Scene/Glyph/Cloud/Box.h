#ifndef OBJECT_BOX_H
#define OBJECT_BOX_H

#include "../Base/Glyph_source.h"


class Box : public Glyph_source
{
public:
  //Constructor / Destructor
  Box();
  ~Box();

public:
  //Main function
  void create_glyph();
  void update_box();

  //Subfunction
  void build_box_location();
  void build_box_color();
  void compute_box_MinMax(Cloud* cloud, vec3 min_perc, vec3 max_perc);

  inline Glyph* get_glyph(){return box;}

private:
  Glyph* box;

  vec4 color;
  int width;
};

#endif
