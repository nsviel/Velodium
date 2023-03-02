#include "Graph.h"

#include "../Node_scene.h"

#include "../Base/Object_base.h"
#include "../Base/Cloud_base.h"
#include "../Base/Mesh_base.h"


//Constructor / Destructor
Graph::Graph(Node_scene* node){
  //---------------------------

  this->list_object = new list<Object_base*>;
  this->list_cloud = new list<Cloud_base*>;
  this->list_mesh = new list<Mesh_base*>;

  //---------------------------
}
Graph::~Graph(){}

void Graph::insert_loaded_cloud(Cloud* cloud){
  Cloud_base* cloud_base = new Cloud_base();
  //Subset* subset = cloud->subset_selected;
  //---------------------------

  cloud_base->ID_perma = cloud->ID;
  cloud_base->ID_order = cloud->oID;
  cloud_base->nb_subset = cloud->nb_subset;
  cloud_base->nb_point = cloud->nb_point;
  cloud_base->name = cloud->name;

  /*cloud_base->subset_selected = cloud->subset_selected;
  cloud_base->subset = cloud->subset;
  cloud_base->subset_voxel = cloud->subset_voxel;
  cloud_base->subset_buffer = cloud->subset_buffer;
  cloud_base->subset_init = cloud->subset_init;*/

  //cloud_base->xyz = subset->xyz;
  //cloud_base->rgb = subset->rgb;

  //cloud_base->vbo_xyz = subset->vbo_xyz;


  glGenVertexArrays(1, &cloud_base->vao);
  glBindVertexArray(cloud_base->vao);

  glGenBuffers(1, &cloud_base->vbo_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, cloud_base->vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, cloud_base->xyz.size()*sizeof(glm::vec3), &cloud_base->xyz[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &cloud_base->vbo_rgb);
  glBindBuffer(GL_ARRAY_BUFFER, cloud_base->vbo_rgb);
  glBufferData(GL_ARRAY_BUFFER, cloud_base->rgb.size()*sizeof(glm::vec4), &cloud_base->rgb[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);



  //---------------------------
  list_cloud->push_back(cloud_base);
}
void Graph::draw_all(){
  //---------------------------

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  for(int i=0; i<list_cloud->size(); i++){
    Cloud_base* cloud = *next(list_cloud->begin(), i);
    cloud->draw();
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  //---------------------------
}
