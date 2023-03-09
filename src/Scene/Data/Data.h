#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "../Base/struct_cloud.h"
#include "../Base/struct_glyph.h"
#include "../Base/struct_mesh.h"
#include "../Base/struct_light.h"

#include <string>
#include <list>


class Data
{
public:
  //Create data instance on first use
  static Data* get_instance() {
      static Data instance;
      return &instance;
  }

  inline std::list<Cloud*>* get_list_cloud(){return list_cloud;}
  inline std::list<Light_*>* get_list_light(){return list_light;}

private:
  Data();
  Data(const Data&) = delete; // disable copy constructor
  Data& operator=(const Data&) = delete; // disable copy assignment operator

private:
  std::list<Object_*>* list_object;
  std::list<Cloud*>* list_cloud;
  std::list<Mesh*>* list_mesh;
  std::list<Light_*>* list_light;
  std::list<Glyph*>* list_glyph;
};

#endif
