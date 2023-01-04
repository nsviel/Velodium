#include "Octree.h"

#include "../../../../Specific/fct_math.h"


//Constructor / destructor
Octree::Octree(){
  //---------------------------

  this->octree_color = vec4(1, 1, 1, 0.7);
  this->nb_cell = 4;

  this->create_octree();

  //---------------------------
}
Octree::~Octree(){
  //---------------------------

  delete octree;

  //---------------------------
}

void Octree::create_octree(){
  octree = new Glyph();
  //---------------------------

  //Create glyph
  octree->name = "octree";
  octree->draw_width = 2;
  octree->visibility = true;
  octree->draw_type = "line";
  octree->permanent = true;
  octree->color_unique = octree_color;

  //---------------------------
}
void Octree::update_octree(Subset* subset){
  vector<vec3>& XYZ_octree = octree->location;
  vector<vec4>& RGB_octree = octree->color;
  vector<vec3>& XYZ_subset = subset->xyz;
  //---------------------------

  // Convert point vector to insert into list
  list<int> list_xyz;
  for(int i=0; i<XYZ_octree.size(); i++){
    list_xyz.push_back(i);
  }

  // Parameters
  XYZ_octree.clear();
  RGB_octree.clear();

  //Manager garbage

  Cube* cube = new Cube();
  Tree tree;

  cube->min = fct_min_vec3(XYZ_subset);
  cube->max = fct_max_vec3(XYZ_subset);
  cube->center = fct_centroid(cube->min, cube->max);
  cube->idx = list_xyz;

  tree.xyz = compute_cube_location(cube->min, cube->max);
  tree.rgb = compute_cube_color(cube->xyz.size());

  this->compute_cube_division(tree, cube, XYZ_subset);

  for(int i=0; i<8; i++){
    Cube* cube_child = cube->child[i];
    this->compute_cube_division(tree, cube_child, XYZ_subset);
  }

  tree.root = cube;

  XYZ_octree = tree.xyz;
  RGB_octree = tree.rgb;


  //---------------------------
}
vector<vec3> Octree::compute_cube_location(vec3 min, vec3 max){
  vector<vec3> XYZ;
  //---------------------------

  XYZ.push_back(vec3(min.x, min.y, min.z));
  XYZ.push_back(vec3(max.x, min.y, min.z));
  XYZ.push_back(vec3(max.x, min.y, min.z));
  XYZ.push_back(vec3(max.x, max.y, min.z));
  XYZ.push_back(vec3(max.x, max.y, min.z));
  XYZ.push_back(vec3(min.x, max.y, min.z));
  XYZ.push_back(vec3(min.x, max.y, min.z));
  XYZ.push_back(vec3(min.x, min.y, min.z));

  XYZ.push_back(vec3(min.x, min.y, max.z));
  XYZ.push_back(vec3(max.x, min.y, max.z));
  XYZ.push_back(vec3(max.x, min.y, max.z));
  XYZ.push_back(vec3(max.x, max.y, max.z));
  XYZ.push_back(vec3(max.x, max.y, max.z));
  XYZ.push_back(vec3(min.x, max.y, max.z));
  XYZ.push_back(vec3(min.x, max.y, max.z));
  XYZ.push_back(vec3(min.x, min.y, max.z));

  XYZ.push_back(vec3(min.x, min.y, min.z));
  XYZ.push_back(vec3(min.x, min.y, max.z));
  XYZ.push_back(vec3(max.x, min.y, min.z));
  XYZ.push_back(vec3(max.x, min.y, max.z));
  XYZ.push_back(vec3(max.x, max.y, min.z));
  XYZ.push_back(vec3(max.x, max.y, max.z));
  XYZ.push_back(vec3(min.x, max.y, min.z));
  XYZ.push_back(vec3(min.x, max.y, max.z));

  return XYZ;
  //---------------------------
}
vector<vec4> Octree::compute_cube_color(int size){
  vector<vec4> RGB;
  //---------------------------

  for(int i=0; i<size; i++){
    RGB.push_back(octree_color);
  }

  //---------------------------
  return RGB;
}
void Octree::compute_cube_division(Tree& tree, Cube* cube_parent, vector<vec3>& point){
  vector<Cube> cube_vec;
  //---------------------------

  for(int i=0; i<8; i++){
    Cube* cube = new Cube();
    if(i == 0){
      cube->min = cube_parent->min;
      cube->max = cube_parent->center;
    }
    else if(i == 1){
      cube->min = cube_parent->min;
      cube->min.x = cube_parent->center.x;
      cube->max = cube_parent->center;
      cube->max.x = cube_parent->max.x;
    }
    else if(i == 2){
      cube->min = cube_parent->min;
      cube->min.y = cube_parent->center.y;
      cube->max = cube_parent->center;
      cube->max.y = cube_parent->max.y;
    }
    else if(i == 3){
      cube->min = cube_parent->center;
      cube->min.z = cube_parent->min.z;
      cube->max = cube_parent->max;
      cube->max.z = cube_parent->center.z;
    }
    else if(i == 4){
      cube->min = cube_parent->min;
      cube->min.z = cube_parent->center.z;
      cube->max = cube_parent->center;
      cube->max.z = cube_parent->max.z;
    }
    else if(i == 5){
      cube->min = cube_parent->center;
      cube->min.y = cube_parent->min.y;
      cube->max = cube_parent->center;
      cube->max.x = cube_parent->max.x;
    }
    else if(i == 6){
      cube->min = cube_parent->center;
      cube->min.x = cube_parent->min.x;
      cube->max = cube_parent->max;
      cube->max.x = cube_parent->center.x;
    }
    else if(i == 7){
      cube->min = cube_parent->center;
      cube->max = cube_parent->max;
    }
    cube->center = fct_centroid(cube->min, cube->max);
    cube->xyz = compute_cube_location(cube->min, cube->max);
    cube->rgb = compute_cube_color(cube->xyz.size());

    tree.xyz.insert(tree.xyz.end(), cube->xyz.begin(), cube->xyz.end());
    tree.rgb.insert(tree.rgb.end(), cube->rgb.begin(), cube->rgb.end());

    cube_parent->child.push_back(cube);
  }

  //---------------------------
}
