#ifndef SLAM_OBJECT_LOCALMAP_H
#define SLAM_OBJECT_LOCALMAP_H

#include "../src/struct_voxelMap.h"
#include "../Base/SLAM_object_base.h"


class Localmap : public SLAM_object_base
{
public:
  //Constructor / Destructor
  Localmap();
  ~Localmap();

public:
  void clear();
  void create_localmap();
  void create_localcloud();
  void update_localmap(slamap* map);
  void update_localcloud(slamap* map);

  inline Glyph* get_localmap(){return localmap;}
  inline Glyph* get_localcloud(){return localcloud;}

private:
  Glyph* localmap;
  Glyph* localcloud;

  vec4 color;
};

#endif
