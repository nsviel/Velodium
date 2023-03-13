#include "Graph.h"
#include "Scene.h"

#include "../Node_scene.h"


//Constructor / Destructor
Graph::Graph(Node_scene* node){
  //---------------------------

  this->sceneManager = node->get_sceneManager();

  //---------------------------
}
Graph::~Graph(){}

//Selection
void Graph::selection_setCloud(int ID){
  list<Collection*>* list_collection= sceneManager->get_list_collection();
  //---------------------------

  for (int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    if(collection->ID_col_order == ID){
      sceneManager->set_selected_collection(collection);
      //sceneManager->update_glyph(collection);
    }
  }

  //---------------------------
}
void Graph::selection_setCloud(Collection* collection){
  //---------------------------

  sceneManager->set_selected_collection(collection);
  //sceneManager->update_glyph(collection);

  //---------------------------
}
void Graph::selection_setSubset(Collection* collection, int ID){
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);

    if(i == ID){
      collection->ID_obj_selected = ID;
      cloud->is_visible = true;
    }else{
      cloud->is_visible = false;
    }

  }

  //---------------------------
}
void Graph::selection_setNext(){
  list<Collection*>* list_collection= sceneManager->get_list_collection();
  Collection* selected_col = sceneManager->get_selected_collection();;
  if(list_collection->size() == 0) return;
  //---------------------------

  if(selected_col->ID_col_order + 1 < list_collection->size()){
    selected_col = *next(list_collection->begin(),selected_col->ID_col_order + 1);
  }
  else{
    selected_col = *next(list_collection->begin(),0);
  }
  sceneManager->update_MinMax_col(selected_col);
  //sceneManager->update_glyph(selected_col);
  sceneManager->set_selected_collection(selected_col);

  //---------------------------
}
void Graph::selection_cloudByName(string name){
  list<Collection*>* list_collection= sceneManager->get_list_collection();
  //---------------------------

  for (int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    if(collection->name == name){
      sceneManager->set_selected_collection(collection);
      //sceneManager->update_glyph(collection);
    }
  }

  //---------------------------
}
