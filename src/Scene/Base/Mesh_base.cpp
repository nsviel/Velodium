#include "Mesh_base.h"


//Constructor / Destructor
Mesh_base::Mesh_base(){
  //---------------------------

  this->init_object();

  //---------------------------
}
Mesh_base::~Mesh_base(){}

void Mesh_base::init(){
  //---------------------------

  this->object_type = "mesh";

  //---------------------------
}
