#include "Data.h"


Data::Data(){
  //---------------------------

  this->list_object = new std::list<Object_*>;
  this->list_cloud = new std::list<Cloud*>;
  this->list_mesh = new std::list<Mesh*>;
  this->list_light = new std::list<Light_*>;
  this->list_glyph = new std::list<Glyph*>;

  this->cloud_selected = nullptr;

  //---------------------------
}
