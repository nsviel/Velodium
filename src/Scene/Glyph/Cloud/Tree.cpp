#include "Tree.h"

#include "../../../Specific/Function/fct_math.h"
#include "../../../Operation/Color/Color.h"
#include "../../../Operation/Function/Octree.h"


//Constructor / destructor
Tree::Tree(){
  //---------------------------

  this->octreeManager = new Octree();

  this->name = "tree";
  this->is_visible = false;
  this->tree_color = vec4(1, 1, 1, 0.7);
  this->tree_level = 6;

  //---------------------------
}
Tree::~Tree(){}

void Tree::create_glyph(Cloud* cloud){
  Glyph tree;
  //---------------------------

  //Create glyph
  tree.name = "tree";
  tree.draw_line_width = 2;
  tree.is_visible = is_visible;
  tree.draw_type_name = "line";
  tree.is_permanent = true;
  tree.unicolor = tree_color;

  //---------------------------
  cloud->glyphs.insert({"tree", tree});
}
void Tree::update_tree(Cloud* cloud){
  //---------------------------

  if(is_visible){
    octreeManager->create_octree(cloud, tree_level);
  }

  //---------------------------
}
