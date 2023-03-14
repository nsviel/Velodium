#include "Data.h"


Data::Data(){
  //---------------------------

  this->list_col_object = new std::list<Collection*>;
  this->list_col_glyph = new std::list<Collection*>;

  this->selected_col = nullptr;

  this->create_collection_glyph("glyph_scene");
  this->create_collection_glyph("glyph_object");
  this->create_collection_glyph("light");

  //---------------------------
}

//Main function
void Data::update_ID_order(){
  //---------------------------

  for(int i=0; i<list_col_object->size(); i++){
    Collection* collection = *next(list_col_object->begin(),i);
    if(collection->ID_col_order != i) collection->ID_col_order = i;
  }

  //---------------------------
}
void Data::create_collection_glyph(std::string name){
  //---------------------------

  //Check if there is not any other collection with this name
  for(int i=0; i<list_col_glyph->size(); i++){
    Collection* col = *next(list_col_glyph->begin(), i);

    if(col->name == name){
      return;
    }
  }

  //If ok, create it
  Collection* col_new = new Collection(name);
  list_col_glyph->push_back(col_new);

  //---------------------------
}
Collection* Data::create_collection_object(std::string name){
  //---------------------------

  //Check if there is not any other collection with this name
  for(int i=0; i<list_col_object->size(); i++){
    Collection* col = *next(list_col_object->begin(), i);

    if(col->name == name){
      return col;
    }
  }

  //If ok, create it
  Collection* col_new = new Collection(name);
  list_col_object->push_back(col_new);
  this->selected_col = col_new;

  //---------------------------
  return col_new;
}
void Data::insert_new_collection(Collection* collection){
  //---------------------------

  list_col_object->push_back(collection);
  this->selected_col = collection;
  this->update_ID_order();

  //---------------------------
}

//Assesseurs
void Data::set_selected_collection(int ID_order){
  //---------------------------

  //Check for emptyness
  if(list_col_object->size() == 0){
    this->selected_col = nullptr;
    return;
  }

  //Check for end list new selected collection
  if(ID_order >= list_col_object->size()){
    ID_order = 0;
  }

  this->selected_col = *next(list_col_object->begin(), ID_order);

  //---------------------------
}
void Data::set_selected_collection(Collection* collection){
  //---------------------------

  selected_col = collection;

  //---------------------------
}
Collection* Data::get_collection_byName(std::string list_name, std::string querry){
  std::list<Collection*>* list;
  //---------------------------

  if(list_name == "object"){
    list = list_col_object;
  }else if(list_name == "glyph"){
    list = list_col_glyph;
  }

  //Check if there is not any other collection with this name
  for(int i=0; i<list->size(); i++){
    Collection* col = *next(list->begin(), i);

    if(col->name == querry){
      return col;
    }
  }

  //---------------------------
  return nullptr;
}
