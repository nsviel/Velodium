#ifndef OBJECT_LOCALMAP_H
#define OBJECT_LOCALMAP_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Localmap
{
public:
  //Constructor / Destructor
  Localmap();
  ~Localmap();

public:
  void create_localmap();
  void update_localmap();

  inline void set_visibility(bool value){this->visibility = value;}
  inline bool* get_visibility(){return &visibility;}

private:
  Glyph* localmap;
  bool visibility;
  vec4 color;
};

#endif
