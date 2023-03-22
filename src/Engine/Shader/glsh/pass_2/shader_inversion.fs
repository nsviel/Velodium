#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color;


void main(){
  //---------------------------

  vec4 color_rgba = vec4(vec3(1.0 - texture(tex_color, vs_tex_coord)), 1.0);
  out_color = vec4(color_rgba);

  //---------------------------
}
