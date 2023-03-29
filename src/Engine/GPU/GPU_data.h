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
  void convert_draw_type_byName(Object_* object);

  //Buffer generation
  void gen_object_buffers(Object_* object);
  void gen_buffer_location(Object_* object);
  void gen_buffer_color(Object_* object);
  void gen_buffer_normal(Object_* object);
  void gen_buffer_uv(Object_* object);
  int gen_texture(unsigned char* tex_data, int w, int h, int nb);

  //Buffer update
  void update_buffer_location(Object_* object);
  void update_buffer_color(Object_* object);
  void update_buffer_normal(Object_* object);
  void update_buffer_texture(Object_* object);

  //Buffer binding / unbinding
  void bind_texture(vector<int> tex_ID);
  void unbind_object(Object_* object);

private:

};

#endif
