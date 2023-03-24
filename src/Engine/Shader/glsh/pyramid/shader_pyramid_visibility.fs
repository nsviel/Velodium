#version 330 core
#define PI 3.1415926538

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_py_0;
/*uniform sampler2D tex_py_1;
uniform sampler2D tex_py_2;
uniform sampler2D tex_py_3;*/

uniform int GL_WIDTH;
uniform int GL_HEIGHT;


void main(){
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);
  //---------------------------

  //for level 0
  int NN_SIZE = 1;


  //Comoute neighbor sector index
  float theta = 0;
  for(int i=-NN_SIZE; i<=NN_SIZE; i++){
    for(int j=-NN_SIZE; j<NN_SIZE; j++){
      float l = pixel_size.x * i;
      float k = pixel_size.y *j;
      float alpha = atan(l, k);

      if(alpha < 0){
        alpha = alpha + 2 * PI;
      }
      theta = (8 * alpha) / (2 * PI);
    }
  }






  //for level 1
  //vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  //vec2 cube_pos = (floor(tex_coord_dim / NN_SIZE) * NN_SIZE) / vec2(GL_WIDTH, GL_HEIGHT);


  //---------------------------
  out_color = vec4(theta/7, 0, 0, 1);
}
