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
  list<Cloud*>* list_cloud= sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    if(cloud->ID_order == ID){
      cloud_selected = cloud;
      this->update_glyph(cloud_selected);
    }
  }

  //---------------------------
}
void Graph::selection_setCloud(Cloud* cloud){
  //---------------------------

  cloud_selected = cloud;
  this->update_glyph(cloud_selected);

  //---------------------------
}
void Graph::selection_setSubset(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);

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
  list<Cloud*>* list_cloud= sceneManager->get_list_cloud();
  if(list_cloud->size() == 0) return;
  //---------------------------

  if(cloud_selected->ID_order + 1 < list_cloud->size()){
    cloud_selected = *next(list_cloud->begin(),cloud_selected->ID_order + 1);
  }
  else{
    cloud_selected = *next(list_cloud->begin(),0);
  }
  this->update_cloud_MinMax(cloud_selected);
  this->update_glyph(cloud_selected);

  //---------------------------
}
void Graph::selection_cloudByName(string name){
  list<Cloud*>* list_cloud= sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    if(cloud->name == name){
      cloud_selected = cloud;
      this->update_glyph(cloud_selected);
    }
  }

  //---------------------------
}
