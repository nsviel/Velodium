#version 330 core

in VS_OUT{
  vec4 color;
  vec2 texture;
} fs_in;

out vec4 fs_color;

uniform float near;
uniform float far;


float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
  float depth = linearize_depth(gl_FragCoord.z) / far;
  fs_color = vec4(vec3(depth), 1.0);
}
