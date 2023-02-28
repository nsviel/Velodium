#include "Drawer.h"

#include "../Node_engine.h"
#include "../OpenGL/Texture.h"


//Constructor / Destructor
Drawer::Drawer(Node_engine* node){
  //---------------------------

  this->texManager = node->get_texManager();

  //---------------------------
}
Drawer::~Drawer(){}

//Main function
void Drawer::draw_mesh(Subset* subset, string draw_type){
  bool with_texture = *texManager->get_with_texture();
  //---------------------------

  if(subset->visibility){
    // If any, activate attached texture
    if(with_texture && subset->has_texture){
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, subset->texture_ID);
    }

    // Bind the glyph VAO and draw data
    glBindVertexArray(subset->VAO);
    if(draw_type == "point"){
      glDrawArrays(GL_POINTS, 0, subset->xyz.size());
    }
    else if(draw_type == "line"){
      glDrawArrays(GL_LINES, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle"){
      glDrawArrays(GL_TRIANGLES, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle_strip"){
      glDrawArrays(GL_TRIANGLE_STRIP, 0, subset->xyz.size());
    }
    else if(draw_type == "triangle_fan"){
      glDrawArrays(GL_TRIANGLE_FAN, 0, subset->xyz.size());
    }
    else if(draw_type == "quad"){
      glDrawArrays(GL_QUADS, 0, subset->xyz.size());
    }
    else if(draw_type == "quad_strip"){
      glDrawArrays(GL_QUAD_STRIP, 0, subset->xyz.size());
    }
    else if(draw_type == "polygon"){
      glDrawArrays(GL_POLYGON, 0, subset->xyz.size());
    }
    else{
      glDrawArrays(GL_POINTS, 0, subset->xyz.size());
    }
    glBindVertexArray(0);

    //Desactivate texture
    if(with_texture && subset->has_texture){
      glBindTexture(GL_TEXTURE_2D, 2);
    }
  }

  //---------------------------
}
