#ifndef GPU_PULLPUSH_H
#define GPU_PULLPUSH_H

#include "../GPU/GPU_fbo.h"
#include "../../common.h"

class Node_engine;


class GPU_pullpush
{
public:
  GPU_pullpush(Node_engine* node_engine);
  ~GPU_pullpush();

public:
  //Main function
  void bind_pullpush(Object_* canvas);

  //Subfunction
  void unbind_fboAndTexture(int nb_tex);

private:

};

#endif
