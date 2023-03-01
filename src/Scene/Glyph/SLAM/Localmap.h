#ifndef OBJECT_LOCALMAP_H
#define OBJECT_LOCALMAP_H

#include "../../Base/Object_base.h"


class Localmap : public Object_base
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
