#include "Graph.h"

#include "../Node_scene.h"

#include "../Base/Mesh_base.h"
#include "../Base/Cloud_base.h"


//Constructor / Destructor
Graph::Graph(Node_scene* node){
  //---------------------------

  this->list_mesh = new list<Mesh_base*>;
  this->list_cloud = new list<Cloud_base*>;

  //---------------------------
}
Graph::~Graph(){}
