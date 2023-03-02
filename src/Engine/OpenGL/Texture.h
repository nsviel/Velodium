#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../Scene/Struct/struct_misc.h"

#include "../../common.h"

class GPU_transfert;

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
  int load_texture(string filePath, string name);
  void apply_texture(Subset* subset, string tex_path);

  inline bool* get_with_texture(){return &with_texture;}

private:
  GPU_transfert* gpuManager;
  
  list<Texture_obj*>* list_texture;
  unsigned int cubemapTexture;
  GLuint ID;
  string Name;
  int width = 0;
  int height = 0;
  bool with_texture;
};

#endif
