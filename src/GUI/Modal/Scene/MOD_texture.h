#ifndef MOD_TEXTURE_H
#define MOD_TEXTURE_H

#include "../../../common.h"

class Node_engine;
class Texture;
class Scene;


class MOD_texture
{
public:
  //Constructor / Destructor
  MOD_texture(Node_engine* node_engine);
  ~MOD_texture();

public:
  void window_texture();
  void texture_operation();

private:
  Scene* sceneManager;
  Texture* texManager;

  int item_width;
};

#endif
