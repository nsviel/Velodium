#ifndef GPU_DATA_H
#define GPU_DATA_H

#include "../../common.h"


class GPU_data
{
public:
  GPU_data();
  ~GPU_data();

public:
  //Generic function
  void gen_vao(Object_* object);
  void draw_object(Object_* object);

  //Buffer binding
  void bind_object_buffers(Object_* object);
  void bind_buffer_location(Object_* object);
  void bind_buffer_color(Object_* object);
  void bind_buffer_texture(Object_* object);

  //Buffer update & unbind
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void unbind_object(Object_* object);

  //Misc function
  int bind_texture(unsigned char* tex_data, int w, int h, int nb);
  void convert_draw_type_byName(Object_* object);

private:

};

#endif
