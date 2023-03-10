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
  list<Collection*>* list_cloud= sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Collection* cloud = *next(list_cloud->begin(),i);
    if(cloud->ID_order == ID){
      sceneManager->set_selected_cloud(cloud);
      sceneManager->update_glyph(cloud);
    }
  }

  //---------------------------
}
void Graph::selection_setCloud(Collection* cloud){
  //---------------------------

  sceneManager->set_selected_cloud(cloud);
  sceneManager->update_glyph(cloud);

  //---------------------------
}
void Graph::selection_setSubset(Collection* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Cloud* subset = *next(cloud->subset.begin(), i);

    if(i == ID){
      cloud->ID_selected = ID;
      subset->is_visible = true;
    }else{
      subset->is_visible = false;
    }

  }

  //---------------------------
}
void Graph::selection_setNext(){
  list<Collection*>* list_cloud= sceneManager->get_list_cloud();
  Collection* cloud_selected = sceneManager->get_selected_cloud();;
  if(list_cloud->size() == 0) return;
  //---------------------------

  if(cloud_selected->ID_order + 1 < list_cloud->size()){
    cloud_selected = *next(list_cloud->begin(),cloud_selected->ID_order + 1);
  }
  else{
    cloud_selected = *next(list_cloud->begin(),0);
  }
  sceneManager->update_cloud_MinMax(cloud_selected);
  sceneManager->update_glyph(cloud_selected);
  sceneManager->set_selected_cloud(cloud_selected);

  //---------------------------
}
void Graph::selection_cloudByName(string name){
  list<Collection*>* list_cloud= sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Collection* cloud = *next(list_cloud->begin(),i);

    if(cloud->name == name){
      sceneManager->set_selected_cloud(cloud);
      sceneManager->update_glyph(cloud);
    }
  }

  //---------------------------
}
