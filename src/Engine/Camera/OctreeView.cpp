#include "OctreeView.h"

#include "../../Specific/Function/fct_math.h"


//Constructor / Destructor
OctreeView::OctreeView(){
  //---------------------------


  //---------------------------
}
OctreeView::~OctreeView(){}

//Main functions
void OctreeView::construct_octree(Cloud* cloud){
  //---------------------------

  struct Cube{
    int level;
    vec3 min;
    vec3 max;
  };

  Cube cube;

  vec3 min = fct_min_vec3(cloud->xyz);
  vec3 max = fct_max_vec3(cloud->xyz);




  //---------------------------
}
