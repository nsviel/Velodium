#version 330 core

layout (location = 1) out vec4 out_position;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;

uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform int NN_SIZE;


void main(){
  float pixel_depth = texture(tex_depth, vs_tex_coord).r;
  //---------------------------

  //Get pixel size
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);

  //Retrieve cube position
  vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  vec2 cube_pos = (floor(tex_coord_dim / NN_SIZE) * NN_SIZE) / vec2(GL_WIDTH, GL_HEIGHT);

  //Check if current pixel is the nearest
  float pixel_nearest = 1;
  vec4 pixel_nearest_pos = vec4(1);
  int block_size = NN_SIZE / 2;
  for(float i=0; i<NN_SIZE; i = i + block_size){
    for(float j=0; j<NN_SIZE; j = j + block_size){
      //Neigbor data
      vec2 NN_coord = cube_pos + vec2(pixel_size.x * i, pixel_size.y * j);
      float NN_depth = texture(tex_depth, NN_coord).r;
      vec4 NN_position = texture(tex_position, NN_coord);

      if(NN_depth < pixel_nearest){
        pixel_nearest = NN_depth;
        pixel_nearest_pos = NN_position;
      }
    }
  }

  //---------------------------
  gl_FragDepth = pixel_nearest;
  out_position = pixel_nearest_pos;
}
