#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"


//Constructor / Destructor
Texture::Texture(){
  //---------------------------

  this->list_texture = new list<Texture_obj*>;
  this->with_texture = true;

  //---------------------------
}
Texture::~Texture(){}

//Texture management
int Texture::load_texture(string path, string name){
  int tex_w = 0;
  int tex_h = 0;
  int tex_nb_channel = 0;
  //---------------------------

  // Load from file
  unsigned char* tex_data = stbi_load(path.c_str(), &tex_w, &tex_h, &tex_nb_channel, 0);
  if(tex_data == NULL){
    cout<<"Can't load texture file"<<endl;
    return -1;
  }

  // Create a OpenGL texture identifier
  GLuint tex_ID;
  glGenTextures(1, &tex_ID);
  glBindTexture(GL_TEXTURE_2D, tex_ID);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Upload pixels into texture
  //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  if(tex_nb_channel == 3){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
  }else if(tex_nb_channel == 4){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(tex_data);

  //Create and store texture
  Texture_obj* texture = new Texture_obj();
  texture->ID = tex_ID;
  texture->name = name;
  texture->width = width;
  texture->height = height;
  texture->nb_channel = tex_nb_channel;
  list_texture->push_back(texture);

  //---------------------------
  return tex_ID;
}
void Texture::apply_texture(Subset* subset, string tex_path){
  //---------------------------

  this->load_texture(tex_path, "texture");
  glBindTexture(GL_TEXTURE_2D, subset->texture_ID);


  //---------------------------
}
