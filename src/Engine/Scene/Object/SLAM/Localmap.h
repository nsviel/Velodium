#ifndef OBJECT_LOCALMAP_H
#define OBJECT_LOCALMAP_H

#include "../../../Data/struct_glyph.h"
#include "../../../Data/struct_voxelMap.h"

#include "../../../../common.h"


class Localmap
{
public:
  //Constructor / Destructor
  Localmap();
  ~Localmap();

public:
  void create_localmap();
  void update_localmap(slamap* slam_map);

  inline void set_visibility(bool value){this->visibility = value; this->localmap->visibility = value;}
  inline bool* get_visibility(){return &visibility;}
  inline Glyph* get_glyph(){return localmap;}

private:
  Glyph* localmap;
  bool visibility;
  vec4 color;
};

#endif
