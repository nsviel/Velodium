#version 330 core
#define PI 3.1415926538

layout (location = 1) out vec4 out_position;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;
uniform float Z_NEAR;
uniform float Z_FAR;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform mat4 VIEW;


float compute_depth_normalized(float depth){
  //---------------------------

  float depth_norm = (2.0 * Z_NEAR) / (Z_FAR + Z_NEAR - depth * (Z_FAR - Z_NEAR));

  //---------------------------
  return depth_norm;
}
vec3 unproject(vec2 coord_frag){
  //---------------------------

  //Raster space to NDC space
  vec2 coord_ndc;
  coord_ndc.x = (coord_frag.x) / GL_WIDTH;
  coord_ndc.y = (coord_frag.y) / GL_HEIGHT;

  //Coord in NDC space to clip coordinate
  vec2 coord_clip;
  coord_clip.x = 2 * coord_ndc.x - 1;
  coord_clip.y = 1 - 2 * coord_ndc.y;

  //Clip to view  space
  float ratio = GL_WIDTH / GL_HEIGHT;
  vec4 coord_view;
  coord_view.x = coord_clip.x * tan(65 / 2 * 2 * PI) * ratio;
  coord_view.y = coord_clip.y * tan(65 / 2 * 2 * PI);
  coord_view.z = -1;
  coord_view.w = 1;

  //View space to world space
  mat4 view_inv = inverse(VIEW);
  vec4 coord_world = view_inv * coord_view;

  vec3 fct_out  = vec3(coord_world);

  //---------------------------
  return fct_out;
}

void main(){
  //---------------------------

  //Depth
  float depth = texture(tex_depth, vs_tex_coord).r;
  float depth_norm = compute_depth_normalized(depth);
  if(depth_norm < 0.95){
    gl_FragDepth = depth_norm;
    out_position = texture(tex_position, vs_tex_coord);
    //out_position = vec4(gl_FragDepth,0,0,1);
  }else{
    gl_FragDepth = 1;
    out_position = vec4(1, 1, 1, 1);
  }

  //---------------------------
}
