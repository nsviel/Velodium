#include "GPU_transfert.h"

#include <GL/glew.h>


//Constructor / Destructor
GPU_transfert::GPU_transfert(){
  //---------------------------


  //---------------------------
}
GPU_transfert::~GPU_transfert(){}

//Generic functions
void GPU_transfert::gen_vao(Object_* object){
  //---------------------------

  glGenVertexArrays(1, &object->vao);

  //---------------------------
}
void GPU_transfert::draw_object(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glPointSize(object->draw_point_size);
  glLineWidth(object->draw_line_width);
  glDrawArrays(object->draw_type, 0, object->xyz.size());
  glBindVertexArray(0);

  //---------------------------
}

//Buffer binding
void GPU_transfert::bind_buffer_location(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, object->xyz.size()*sizeof(glm::vec3), &object->xyz[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  //---------------------------
}
void GPU_transfert::bind_buffer_color(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_rgb);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_rgb);
  glBufferData(GL_ARRAY_BUFFER, object->rgb.size()*sizeof(glm::vec4), &object->rgb[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  //---------------------------
}
void GPU_transfert::bind_buffer_texture(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_tex);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_tex);
  glBufferData(GL_ARRAY_BUFFER, object->uv.size()*sizeof(glm::vec2), &object->uv[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}

//Buffer update
void GPU_transfert::update_buffer_location(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, object->xyz.size() * sizeof(glm::vec3), &object->xyz[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void GPU_transfert::update_buffer_color(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_rgb);
  glBufferData(GL_ARRAY_BUFFER, object->rgb.size() * sizeof(glm::vec4), &object->rgb[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}

//Misc functions
int GPU_transfert::bind_texture(unsigned char* tex_data, int w, int h, int nb){
  //---------------------------

  GLuint tex_ID;
  glGenTextures(1, &tex_ID);
  glBindTexture(GL_TEXTURE_2D, tex_ID);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Upload pixels into texture
  if(nb == 3){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
  }else if(nb == 4){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  //---------------------------
  return tex_ID;
}
void GPU_transfert::convert_draw_type_byName(Object_* object){
  //---------------------------

  if(object->draw_type_name == "point"){
    object->draw_type = GL_POINTS;
  }
  else if(object->draw_type_name == "line"){
    object->draw_type = GL_LINES;
  }
  else if(object->draw_type_name == "triangle"){
    object->draw_type = GL_TRIANGLES;
  }
  else if(object->draw_type_name == "triangle_strip"){
    object->draw_type = GL_TRIANGLE_STRIP;
  }
  else if(object->draw_type_name == "triangle_fan"){
    object->draw_type = GL_TRIANGLE_FAN;
  }
  else if(object->draw_type_name == "quad"){
    object->draw_type = GL_QUADS;
  }
  else if(object->draw_type_name == "quad_strip"){
    object->draw_type = GL_QUAD_STRIP;
  }
  else if(object->draw_type_name == "polygon"){
    object->draw_type = GL_POLYGON;
  }
  else{
    object->draw_type = GL_POINTS;
  }

  //---------------------------
}
