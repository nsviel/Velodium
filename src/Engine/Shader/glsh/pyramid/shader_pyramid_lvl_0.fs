#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;


void main(){
  //---------------------------

  vec4 pixel_depth = texture(tex_depth, vs_tex_coord);
  vec4 pixel_position = texture(tex_position, vs_tex_coord);

  //---------------------------
  out_color = vec4(pixel_position.rgb, pixel_depth.r);
}
