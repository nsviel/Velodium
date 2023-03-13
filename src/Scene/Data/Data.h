#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "../Base/struct_collection.h"
#include "../Base/struct_glyph.h"
#include "../Base/struct_mesh.h"
#include "../Base/struct_light.h"

#include <string>
#include <list>

/*
Collection 0: Glyph
Collection 1: Light
Collection n: Cloud
*/


class Data
{
public:
  //Create data instance on first use
  static Data* get_instance() {
    static Data instance;
    return &instance;
  }
  void update_ID_order();
  void create_new_collection(std::string name);
  void set_selected_collection(int ID_order);
  void set_selected_collection(Collection* collection);

  inline std::list<Collection*>* get_list_collection(){return list_collection;}
  inline std::list<Light_*>* get_list_light(){return list_light;}
  inline std::list<Glyph*>* get_list_glyph(){return list_glyph;}

  inline Collection* get_selected_collection(){return selected_col;}

private:
  Data();
  Data(const Data&) = delete; // disable copy constructor
  Data& operator=(const Data&) = delete; // disable copy assignment operator

private:
  std::list<Collection*>* list_collection;
  std::list<Light_*>* list_light;
  std::list<Glyph*>* list_glyph;

  Collection* selected_col;
};

#endif
