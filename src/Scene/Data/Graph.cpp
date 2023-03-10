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
    Collection* cloud = *next(list_collection->begin(),i);
    if(cloud->ID_col_order == ID){
      sceneManager->set_selected_cloud(cloud);
      //sceneManager->update_glyph(cloud);
    }
  }

  //---------------------------
}
void Graph::selection_setCloud(Collection* cloud){
  //---------------------------

  sceneManager->set_selected_cloud(cloud);
  //sceneManager->update_glyph(cloud);

  //---------------------------
}
void Graph::selection_setSubset(Collection* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->nb_object; i++){
    Cloud* subset = (Cloud*)*next(cloud->subset.begin(), i);

    if(i == ID){
      cloud->ID_obj_selected = ID;
      subset->is_visible = true;
    }else{
      subset->is_visible = false;
    }

  }

  //---------------------------
}
void Graph::selection_setNext(){
  list<Collection*>* list_collection= sceneManager->get_list_collection();
  Collection* cloud_selected = sceneManager->get_selected_collection();;
  if(list_collection->size() == 0) return;
  //---------------------------

  if(cloud_selected->ID_col_order + 1 < list_collection->size()){
    cloud_selected = *next(list_collection->begin(),cloud_selected->ID_col_order + 1);
  }
  else{
    cloud_selected = *next(list_collection->begin(),0);
  }
  sceneManager->update_cloud_MinMax(cloud_selected);
  //sceneManager->update_glyph(cloud_selected);
  sceneManager->set_selected_cloud(cloud_selected);

  //---------------------------
}
void Graph::selection_cloudByName(string name){
  list<Collection*>* list_collection= sceneManager->get_list_collection();
  //---------------------------

  for (int i=0; i<list_collection->size(); i++){
    Collection* cloud = *next(list_collection->begin(),i);

    if(cloud->name == name){
      sceneManager->set_selected_cloud(cloud);
      //sceneManager->update_glyph(cloud);
    }
  }

  //---------------------------
}
