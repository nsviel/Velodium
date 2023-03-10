#include "Octree.h"

#include "../Color/Color.h"

#include "../../Specific/Function/fct_math.h"
#include "../../Specific/color.h"


//Constructor / destructor
Octree::Octree(){
  //---------------------------

  this->octree_color = vec4(1, 1, 1, 0.7);
  this->with_rdm_color = false;
  this->nb_level = 6;
  this->root = nullptr;

  //---------------------------
}
Octree::~Octree(){}

//Main function
void Octree::create_octree(Cloud* cloud){
  this->nb_level = 6;
  //---------------------------

tic();
  this->build_root(cloud);
  this->build_octree(root->child);
toc_ms("octree");

  Glyph* glyph = &cloud->glyphs["tree"];
  glyph->xyz = root->xyz;
  glyph->rgb = root->rgb;

  //---------------------------
  this->remove_octree(root);
}
void Octree::create_octree(Cloud* cloud, int level){
  this->nb_level = level;
  //---------------------------

  tic();
  this->build_root(cloud);
  this->build_octree(root->child);
  this->octree_time = toc_ms();

  Glyph* glyph = &cloud->glyphs["tree"];
  glyph->xyz = root->xyz;
  glyph->rgb = root->rgb;

  //---------------------------
  this->remove_octree(root);
}
void Octree::remove_octree(Root* root){
  //---------------------------

  this->remove_cube(root->child);
  delete(root);

  //---------------------------
}

//Subfunctions
void Octree::remove_cube(Cube* cube){
  //---------------------------

  for(int i=0; i<8; i++){
    if(cube->child[i] != nullptr){
      this->remove_cube(cube->child[i]);
    }
  }

  delete(cube);

  //---------------------------
}
void Octree::build_root(Cloud* cloud){
  //---------------------------

  // Create a vector of indexes
  vector<int> idx;
  for(int i=0; i<cloud->xyz.size(); i++){
    idx.push_back(i);
  }

  //Create root cube
  Cube* cube = new Cube();
  cube->min = fct_min_vec3(cloud->xyz);
  cube->max = fct_max_vec3(cloud->xyz);
  cube->center = fct_centroid(cube->min, cube->max);
  cube->level = 0;
  cube->idx_cube = idx;
  cube->idx_child = idx;

  //Initiate octree root
  if(root == nullptr){
    this->root = new Root();
  }else{
    // /this->remove_octree(root);
    this->root = new Root();
  }
  root->xyz_subset = &cloud->xyz;
  root->xyz = compute_cube_location(cube->min, cube->max);
  root->rgb = compute_cube_color(root->xyz.size());
  root->child = cube;

  //Apply color to each level
  if(with_rdm_color){
    for(int i=0; i<nb_level; i++){
      vec4 rgb = random_color();
      root->level_rgb.push_back(rgb);
    }
  }else{
    root->level_rgb.push_back(nord0);
    root->level_rgb.push_back(nord1);
    root->level_rgb.push_back(nord2);
    root->level_rgb.push_back(nord3);
    root->level_rgb.push_back(nord10);
    root->level_rgb.push_back(nord9);
    root->level_rgb.push_back(nord8);
    root->level_rgb.push_back(nord7);
    root->level_rgb.push_back(nord11);
  }

  //---------------------------
}
void Octree::build_octree(Cube* cube){
  if(cube->level < nb_level){
    //---------------------------

    this->compute_cube_division(cube);

    for(int i=0; i<8; i++){
      if(cube->child[i] != nullptr){
        this->build_octree(cube->child[i]);
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
void Octree::compute_cube_division(Cube* cube_parent){
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

    vector<int> idx = compute_idx_from_point(min, max, cube_parent);

    if(idx.size() != 0){
      Cube* cube = new Cube();

      cube->min = min;
      cube->max = max;
      cube->center = fct_centroid(min, max);
      cube->level = cube_parent->level + 1;
      cube->idx_cube = idx;
      cube->idx_child = idx;

      vector<vec3> cube_xyz = compute_cube_location(cube->min, cube->max);
      vector<vec4> cube_rgb = compute_cube_color(cube_xyz.size(), root->level_rgb[cube->level]);
      root->xyz.insert(root->xyz.end(), cube_xyz.begin(), cube_xyz.end());
      root->rgb.insert(root->rgb.end(), cube_rgb.begin(), cube_rgb.end());

      cube_parent->child[i] = cube;
    }else{
      cube_parent->child[i] = nullptr;
    }
  }

  //---------------------------
}
vector<int> Octree::compute_idx_from_point(vec3 min, vec3 max, Cube* cube_parent){
  vector<int>& idx_parent = cube_parent->idx_child;
  vector<vec3>* xyz = root->xyz_subset;
  //---------------------------

  //Prepare index vectors
  vector<int> idx_cube;
  vector<int> idx_parent_new;
  idx_cube.reserve(idx_parent.size());
  idx_parent_new.reserve(idx_parent.size());

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
