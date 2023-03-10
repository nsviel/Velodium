#include "Data.h"


Data::Data(){
  //---------------------------

  this->list_collection = new std::list<Collection*>;
  this->list_light = new std::list<Light_*>;
  this->list_glyph = new std::list<Glyph*>;

  this->cloud_selected = nullptr;

  //---------------------------
}
