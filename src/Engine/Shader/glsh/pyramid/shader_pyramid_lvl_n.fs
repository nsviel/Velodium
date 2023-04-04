#version 330 core

layout (location = 1) out vec4 out_position;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;

uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform int NN_SIZE;


void main(){
  //---------------------------

  //Get pixel coordinate information
  vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  vec2 cube_pos = (floor(tex_coord_dim / nn_size) * nn_size) / vec2(GL_WIDTH, GL_HEIGHT);
  vec2 offset = 0.5 / vec2(GL_WIDTH, GL_HEIGHT);
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);

  //Check if current pixel is the nearest
  float nearest_depth = 1;
  vec4 nearest_posit = vec4(1);
  float block_size = nn_size / 2;
  for(float i=0; i<nn_size; i = i + block_size){
    for(float j=0; j<nn_size; j = j + block_size){
      //Neigbor data
      vec2 NN_coord = cube_pos + vec2(pixel_size.x * i, pixel_size.y * j)  + offset;
      float NN_depth = texture(tex_depth, NN_coord).r;
      vec4 NN_posit = texture(tex_position, NN_coord);

      if(NN_depth < nearest_depth){
        nearest_depth = NN_depth;
        nearest_posit = NN_posit;
      }
    }
  }

  //---------------------------
  gl_FragDepth = nearest_depth;
  out_position = nearest_posit;
}
