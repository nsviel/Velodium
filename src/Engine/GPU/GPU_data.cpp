#include "../GPU/GPU_data.h"

#include <GL/glew.h>


//Constructor / Destructor
GPU_data::GPU_data(){
  //---------------------------


  //---------------------------
}
GPU_data::~GPU_data(){}

//Generic functions
void GPU_data::gen_vao(Object_* object){
  //---------------------------

  glGenVertexArrays(1, &object->vao);

  //---------------------------
}
void GPU_data::draw_object(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glPointSize(object->draw_point_size);
  glLineWidth(object->draw_line_width);
  glDrawArrays(object->draw_type, 0, object->xyz.size());
  glBindVertexArray(0);

  //---------------------------
}
void GPU_data::convert_draw_type_byName(Object_* object){
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

//Buffer generation
void GPU_data::gen_object_buffers(Object_* object){
  //---------------------------

  this->convert_draw_type_byName(object);
  this->gen_buffer_uv(object);
  this->gen_buffer_location(object);
  this->gen_buffer_color(object);
  this->gen_buffer_normal(object);

  //---------------------------
}
void GPU_data::gen_buffer_location(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, object->xyz.size()*sizeof(glm::vec3), &object->xyz[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  //---------------------------
}
void GPU_data::gen_buffer_color(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_rgb);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_rgb);
  glBufferData(GL_ARRAY_BUFFER, object->rgb.size()*sizeof(glm::vec4), &object->rgb[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  //---------------------------
}
void GPU_data::gen_buffer_normal(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_Nxyz);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_Nxyz);
  glBufferData(GL_ARRAY_BUFFER, object->Nxyz.size()*sizeof(glm::vec3), &object->Nxyz[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(3);

  //---------------------------
}
void GPU_data::gen_buffer_uv(Object_* object){
  //---------------------------

  glBindVertexArray(object->vao);
  glGenBuffers(1, &object->vbo_uv);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_uv);
  glBufferData(GL_ARRAY_BUFFER, object->uv.size()*sizeof(glm::vec2), &object->uv[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}
int GPU_data::gen_texture(unsigned char* tex_data, int w, int h, int nb){
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
  }else{
    std::cout<<"[error] gpu transfert nbchannel not taking into account"<<std::endl;
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  //---------------------------
  return tex_ID;
}

//Buffer update
void GPU_data::update_buffer_location(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, object->xyz.size() * sizeof(glm::vec3), &object->xyz[0],  GL_DYNAMIC_DRAW);

  //---------------------------
  this->update_buffer_normal(object);
}
void GPU_data::update_buffer_color(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_rgb);
  glBufferData(GL_ARRAY_BUFFER, object->rgb.size() * sizeof(glm::vec4), &object->rgb[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void GPU_data::update_buffer_normal(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_Nxyz);
  glBufferData(GL_ARRAY_BUFFER, object->Nxyz.size() * sizeof(glm::vec3), &object->Nxyz[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void GPU_data::update_buffer_texture(Object_* object){
  //---------------------------

  glBindBuffer(GL_ARRAY_BUFFER, object->vbo_uv);
  glBufferData(GL_ARRAY_BUFFER, object->uv.size() * sizeof(glm::vec2), &object->uv[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}

//Buffer binding / unbinding
void GPU_data::bind_texture(vector<int> tex_ID){
  //---------------------------

  for(int i=0; i<tex_ID.size(); i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, tex_ID[i]);
  }

  //---------------------------
}
void GPU_data::unbind_object(Object_* object){
  //---------------------------

  glDeleteBuffers(1, &object->vbo_xyz);
  glDeleteBuffers(1, &object->vbo_rgb);
  glDeleteBuffers(1, &object->vbo_uv);
  glDeleteVertexArrays(1, &object->vao);

  //---------------------------
}
