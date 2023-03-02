#ifndef GPU_TRANSFERT_H
#define GPU_TRANSFERT_H

#include "../../common.h"


class GPU_transfert
{
public:
  GPU_transfert();
  ~GPU_transfert();

public:
  //Generic function
  void gen_vao(Object_* object);
  void draw_object(Object_* object);

  //Buffer binding
  void bind_buffer_location(Object_* object);
  void bind_buffer_color(Object_* object);
  void bind_buffer_texture(Object_* object);

  //Buffer unbinding
  void unbind_object(Object_* object);

  //Buffer update
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);

  //Misc function
  int bind_texture(unsigned char* tex_data, int w, int h, int nb);
  void convert_draw_type_byName(Object_* object);

private:

};

#endif
