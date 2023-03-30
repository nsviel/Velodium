#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color_1;
uniform sampler2D tex_color_2;
uniform sampler2D tex_depth_1;
uniform sampler2D tex_depth_2;


void main(){
  //---------------------------

  ivec2 texcoord = ivec2(floor(gl_FragCoord.xy));
  float depth_1 = texelFetch(tex_depth_1, texcoord, 0).r;
  float depth_2 = texelFetch(tex_depth_2, texcoord, 0).r;

  if(depth_1 < depth_2){
    out_color = texelFetch(tex_color_1, texcoord, 0);
    gl_FragDepth = depth_1;
  }else{
    out_color = texelFetch(tex_color_2, texcoord, 0);
    gl_FragDepth = depth_2;
  }

  //---------------------------
}
