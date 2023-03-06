#include "Texture.h"
#include "GPU_transfert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"


//Constructor / Destructor
Texture::Texture(){
  //---------------------------

  this->gpuManager = new GPU_transfert();

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

  int tex_ID = gpuManager->bind_texture(tex_data, tex_w, tex_h, tex_nb_channel);

  //Realease loaded texture
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
  glBindTexture(GL_TEXTURE_2D, subset->tex_ID[0]);


  //---------------------------
}
