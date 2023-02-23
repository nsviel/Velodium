#version 330 core

in vec4 vs_color;
in vec2 vs_texture;

out vec4 fs_color;

uniform sampler2D fs_texture;
uniform bool depth_ON;


void main()
{
  if(depth_ON){
    fs_color = vec4(vec3(gl_FragCoord.z), 1.0);
  }else{
    fs_color = texture(fs_texture, vs_texture) * vs_color;
  }
}
