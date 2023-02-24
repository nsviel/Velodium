#version 330 core

in vec4 vs_color;
in vec2 vs_texture;

out vec4 fs_color;

uniform sampler2D fs_texture;
uniform float near;
uniform float far;
uniform bool depth_ON;



float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
  if(depth_ON){
    float depth = linearize_depth(gl_FragCoord.z) / far;
    fs_color = vec4(vec3(depth), 1.0);
  }else{
    fs_color = texture(fs_texture, vs_texture) * vs_color;
  }
}
