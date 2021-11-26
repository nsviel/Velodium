#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Data/struct_generic.h"

#include "../../common.h"

class Textures
{
public:
  //Constructor / Destructor
  Textures();
  ~Textures();

public:
  //Texture management
  bool load_texture(string filePath, string name);
  Texture* get_TextureByName(string name);

  //Skybox stuff
  void skybox(vec3 camPos);
  void init_skybox();
  unsigned int loadCubemap(vector<string> faces);

  inline list<Texture*>* get_list_texture(){return list_Texture;}

private:
  list<Texture*>* list_Texture;

  Glyph* glyph;
  unsigned int cubemapTexture;
};

#endif
