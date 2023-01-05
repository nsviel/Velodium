#include "Octree.h"

#include "../../../../Specific/fct_math.h"
#include "../../../../Operation/Color/Color.h"

//TODO: Optimization
// - replace as often as possible list by vector
// - Manager garbage



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

  // Create a vector of indexes
  vector<int> idx;
  for(int i=0; i<XYZ_subset.size(); i++){
    idx.push_back(i);
  }

  //Create octree
  Tree tree;
  Cube* cube = new Cube();
  cube->min = fct_min_vec3(XYZ_subset);
  cube->max = fct_max_vec3(XYZ_subset);
  cube->center = fct_centroid(cube->min, cube->max);
  cube->level = 0;
  cube->idx_cube = idx;
  cube->idx_child = idx;

  tree.xyz_subset = &subset->xyz;
  tree.xyz = compute_cube_location(cube->min, cube->max);
  tree.rgb = compute_cube_color(tree.xyz.size());
tic();
  this->build_octree(tree, cube, 6);
toc_ms("octree");
  tree.root = cube;

  XYZ_octree = tree.xyz;
  RGB_octree = tree.rgb;


  //---------------------------
}
void Octree::build_octree(Tree& tree, Cube* cube, int level_max){
  if(cube->level < level_max){
    //---------------------------

    this->compute_cube_division(tree, cube);

    for(int i=0; i<8; i++){
      if(cube->child[i] != nullptr){
        this->build_octree(tree, cube->child[i], level_max);
      }
    }

    //---------------------------
  }
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
vector<vec4> Octree::compute_cube_color(int size, vec4 rgb){
  vector<vec4> RGB;
  //---------------------------

  for(int i=0; i<size; i++){
    RGB.push_back(rgb);
  }

  //---------------------------
  return RGB;
}
void Octree::compute_cube_division(Tree& tree, Cube* cube_parent){
  vector<Cube> cube_vec;
  //---------------------------

  for(int i=0; i<8; i++){
    vec3 min, max;
    if(i == 0){
      min = cube_parent->min;
      max = cube_parent->center;
    }
    else if(i == 1){
      min = cube_parent->min;
      min.x = cube_parent->center.x;
      max = cube_parent->center;
      max.x = cube_parent->max.x;
    }
    else if(i == 2){
      min = cube_parent->min;
      min.y = cube_parent->center.y;
      max = cube_parent->center;
      max.y = cube_parent->max.y;
    }
    else if(i == 3){
      min = cube_parent->center;
      min.z = cube_parent->min.z;
      max = cube_parent->max;
      max.z = cube_parent->center.z;
    }
    else if(i == 4){
      min = cube_parent->min;
      min.z = cube_parent->center.z;
      max = cube_parent->center;
      max.z = cube_parent->max.z;
    }
    else if(i == 5){
      min = cube_parent->center;
      min.y = cube_parent->min.y;
      max = cube_parent->max;
      max.y = cube_parent->center.y;
    }
    else if(i == 6){
      min = cube_parent->center;
      min.x = cube_parent->min.x;
      max = cube_parent->max;
      max.x = cube_parent->center.x;
    }
    else if(i == 7){
      min = cube_parent->center;
      max = cube_parent->max;
    }

    vector<int> idx = compute_idx_from_point(tree, min, max, cube_parent);

    if(idx.size() != 0){
      Cube* cube = new Cube();

      cube->min = min;
      cube->max = max;
      cube->center = fct_centroid(min, max);
      cube->level = cube_parent->level + 1;
      cube->idx_cube = idx;
      cube->idx_child = idx;

      vector<vec3> cube_xyz = compute_cube_location(cube->min, cube->max);
      vector<vec4> cube_rgb = compute_cube_color(cube_xyz.size());
      tree.xyz.insert(tree.xyz.end(), cube_xyz.begin(), cube_xyz.end());
      tree.rgb.insert(tree.rgb.end(), cube_rgb.begin(), cube_rgb.end());

      cube_parent->child[i] = cube;
    }else{
      cube_parent->child[i] = nullptr;
    }
  }

  //---------------------------
}
vector<int> Octree::compute_idx_from_point(Tree& tree, vec3 min, vec3 max, Cube* cube_parent){
  vector<int> idx_cube;
  vector<int> idx_parent_new;
  //---------------------------

  vector<int>& idx_parent = cube_parent->idx_child;
  vector<vec3>* xyz = tree.xyz_subset;

  //Get all cube personnal idx
  for(int i=0; i<idx_parent.size(); i++){
    int id = idx_parent[i];

    if(xyz->at(id).x >= min.x && xyz->at(id).x <= max.x &&
       xyz->at(id).y >= min.y && xyz->at(id).y <= max.y &&
       xyz->at(id).z >= min.z && xyz->at(id).z <= max.z){
      idx_cube.push_back(id);
    }
    else{
      idx_parent_new.push_back(id);
    }
  }

  idx_parent = idx_parent_new;

  //---------------------------
  return idx_cube;
}
