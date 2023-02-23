#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Data/struct_misc.h"

#include "../../common.h"

struct Texture_obj{
  //---------------------------

  GLuint ID;
  std::string name;
  int width = 0;
  int height = 0;
  int nb_channel = 0;

  //---------------------------
};

class Texture
{
public:
  //Constructor / Destructor
  Texture();
  ~Texture();

public:
  //Texture management
  bool load_texture(string filePath, string name);
  void add_texture_to_gpu();
  
  //void skybox(vec3 camPos);
  //void init_skybox();
  //unsigned int loadCubemap(vector<string> faces);
  //Texture* get_TextureByName(string name);
  //inline list<Texture*>* get_list_texture(){return list_Texture;}

private:
  list<Texture_obj*>* list_texture;
  unsigned int cubemapTexture;
  GLuint ID;
  string Name;
  int width = 0;
  int height = 0;
};

#endif
