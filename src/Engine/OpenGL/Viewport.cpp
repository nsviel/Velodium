#include "Viewport.h"

#include "../Configuration/Dimension.h"


//Constructor / Destructor
Viewport::Viewport(Dimension* dimension){
  //---------------------------

  Camera* cameraManager = new Camera(dimension);

  this->dimManager = dimension;
  this->nb_viewport = 2;

  //---------------------------
}
Viewport::~Viewport(){}

void Viewport::update_viewport(){
  //---------------------------

  dimManager->update_window_dim();
  vec2 dim = dimManager->get_glDim();
  vec2 pos = dimManager->get_glPos();

  glViewport(pos.x, pos.y, dim.x/2, dim.y/2);

  //---------------------------
}
void Viewport::update_viewport(int loop_cpt){
  //---------------------------

  dimManager->update_window_dim();
  vec2 dim = dimManager->get_glDim();
  vec2 pos = dimManager->get_glPos();

  if(loop_cpt == 0){
    glViewport(pos.x, pos.y, dim.x, dim.y);

  }else{
    glViewport(pos.x + dim.x/2, pos.y + dim.y/2, dim.x/2, dim.y/2);

  }



  //---------------------------
}
