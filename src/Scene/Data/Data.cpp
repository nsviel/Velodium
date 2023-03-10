#include "Data.h"


Data::Data(){
  //---------------------------

  this->list_collection = new std::list<Collection*>;
  this->list_light = new std::list<Light_*>;
  this->list_glyph = new std::list<Glyph*>;

  this->cloud_selected = nullptr;

  //---------------------------
}

void Data::create_new_collection(std::string name){
  //---------------------------

  //Check if there is not any other collection with this name
  for(int i=0; i<list_collection->size(); i++){
    Collection* col = *next(list_collection->begin(), i);

    if(col->name == name){
      return;
    }
  }

  //If ok, create it
  Collection* col_new = new Collection(name);
  list_collection->push_back(col_new);

  //---------------------------
}
