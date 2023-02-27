#version 330 core

in vec2 vs_tex_coord;
out vec4 fs_color;

uniform sampler2D tex_color;

const float offset = 1.0 / 1000.0;


void main()
{
  vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
  );

  float kernel[9] = float[](
    1,1,1,
    1,-9,1,
    1,1,1
  );

  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
    sampleTex[i] = vec3(texture(tex_color, vs_tex_coord.st + offsets[i]));
  }
  vec3 rgb = vec3(0.0);
  for(int i = 0; i < 9; i++){
    rgb += sampleTex[i] * kernel[i];
  }

  fs_color = vec4(rgb, 1.0);
}
