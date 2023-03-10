#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "../Base/struct_collection.h"
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

  inline std::list<Collection*>* get_list_collection(){return list_collection;}
  inline std::list<Light_*>* get_list_light(){return list_light;}
  inline std::list<Glyph*>* get_list_glyph(){return list_glyph;}

  inline Collection* get_cloud_selected(){return cloud_selected;}
  inline void set_cloud_selected(Collection* cloud){cloud_selected = cloud;}

private:
  Data();
  Data(const Data&) = delete; // disable copy constructor
  Data& operator=(const Data&) = delete; // disable copy assignment operator

private:
  std::list<Object_*>* list_object;
  std::list<Collection*>* list_collection;
  std::list<Mesh*>* list_mesh;
  std::list<Light_*>* list_light;
  std::list<Glyph*>* list_glyph;

  Collection* cloud_selected;
};

#endif
