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
  void create_collection_glyph(std::string name);
  void insert_new_collection(Collection* collection);
  void set_selected_collection(int ID_order);
  void set_selected_collection(Collection* collection);

  Collection* create_collection_object(std::string name);
  Collection* get_collection_byName(std::string list_name, std::string querry);

  inline std::list<Collection*>* get_list_col_object(){return list_col_object;}
  inline std::list<Collection*>* get_list_col_glyph(){return list_col_glyph;}
  inline Collection* get_selected_collection(){return selected_col;}

private:
  Data();
  Data(const Data&) = delete; // disable copy constructor
  Data& operator=(const Data&) = delete; // disable copy assignment operator

private:
  std::list<Collection*>* list_col_object;
  std::list<Collection*>* list_col_glyph;

  Collection* selected_col;
};

#endif
