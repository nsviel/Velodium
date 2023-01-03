#include "Octree.h"

#include "../../Specific/fct_math.h"


//Constructor / Destructor
Octree::Octree(){
  //---------------------------


  //---------------------------
}
Octree::~Octree(){}

//Main functions
void Octree::construct_octree(Subset* subset){
  //---------------------------

  struct Cube{
    int level;
    vec3 min;
    vec3 max;
  };

  Cube cube;

  vec3 min = fct_min_vec3(subset->xyz);
  vec3 max = fct_max_vec3(subset->xyz);




  //---------------------------
}
