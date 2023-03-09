#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "../Base/struct_cloud.h"
#include "../Base/struct_glyph.h"
#include "../Base/struct_mesh.h"
#include "../Base/struct_light.h"

#include <string>
#include <list>


//Cloud are subset containers
struct Data
{
  //---------------------------

  Data();

  std::list<Object_*>* list_object;
  std::list<Cloud*>* list_cloud;
  std::list<Mesh*>* list_mesh;
  std::list<Light_*>* list_light;
  std::list<Glyph*>* list_glyph;

  //---------------------------
};

#endif
