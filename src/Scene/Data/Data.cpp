#include "Data.h"


Data::Data(){
  //---------------------------

  this->list_collection = new std::list<Collection*>;
  this->list_light = new std::list<Light_*>;
  this->list_glyph = new std::list<Glyph*>;

  this->selected_col = nullptr;

  //---------------------------
}

void Data::update_ID_order(){
  //---------------------------

  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    if(collection->ID_col_order != i) collection->ID_col_order = i;
  }

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
void Data::set_selected_collection(int ID_order){
  //---------------------------

  //Check for emptyness
  if(list_collection->size() == 0){
    this->selected_col = nullptr;
    return;
  }

  //Check for end list new selected collection
  if(ID_order >= list_collection->size()){
    ID_order = 0;
  }

  this->selected_col = *next(list_collection->begin(), ID_order);

  //---------------------------
}
void Data::set_selected_collection(Collection* collection){
  //---------------------------

  selected_col = collection;

  //---------------------------
}
