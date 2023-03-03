#include "Tree.h"

#include "../../../Specific/Function/fct_math.h"
#include "../../../Operation/Color/Color.h"
#include "../../../Operation/Function/Octree.h"


//Constructor / destructor
Tree::Tree(){
  //---------------------------

  this->octreeManager = new Octree();

  this->name = "tree";
  this->visibility = false;
  this->tree_color = vec4(1, 1, 1, 0.7);
  this->tree_level = 6;

  //---------------------------
}
Tree::~Tree(){}

void Tree::create_glyph(Subset* subset){
  Glyph tree;
  //---------------------------

  //Create glyph
  tree.name = "tree";
  tree.draw_line_width = 2;
  tree.visibility = visibility;
  tree.draw_type_name = "line";
  tree.permanent = true;
  tree.unicolor = tree_color;

  //---------------------------
  subset->glyphs.insert({"tree", tree});
}
void Tree::update_tree(Subset* subset){
  //---------------------------

  if(visibility){
    octreeManager->create_octree(subset, tree_level);
  }

  //---------------------------
}
