#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../Module/Registration/struct_misc.h"

#include "../../common.h"

class GPU_data;

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

  inline bool* get_with_texture(){return &with_texture;}

private:
  GPU_data* gpuManager;

  list<Texture_obj*>* list_texture;
  bool with_texture;
};

#endif
