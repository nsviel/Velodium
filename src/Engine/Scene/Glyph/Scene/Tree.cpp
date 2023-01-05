#include "Tree.h"

#include "../../../../Specific/fct_math.h"
#include "../../../../Operation/Color/Color.h"

//TODO: Optimization
// - replace as often as possible list by vector
// - Manager garbage
// - put all in a class



//Constructor / destructor
Tree::Tree(){
  //---------------------------

  this->tree_color = vec4(1, 1, 1, 0.7);

  this->create_tree();

  //---------------------------
}
Tree::~Tree(){
  //---------------------------

  delete tree;

  //---------------------------
}

void Tree::create_tree(){
  tree = new Glyph();
  //---------------------------

  //Create glyph
  tree->name = "tree";
  tree->draw_width = 2;
  tree->visibility = true;
  tree->draw_type = "line";
  tree->permanent = true;
  tree->color_unique = tree_color;

  //---------------------------
}
void Tree::update_tree(Subset* subset){
  //---------------------------


  //---------------------------
}
