#version 330 core

layout (location = 0) out vec4 out_color;

in VS_OUT{
  vec4 vertex_color;
  vec3 vertex_normal;
  vec3 vertex_position;
} fs_in;

uniform vec3 light_color;
uniform vec3 light_position;


void main()
{
  //Ambient light
  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_color;

  //Diffuse light
  vec3 norm = normalize(fs_in.vertex_normal);
  vec3 light_direction = normalize(light_position - fs_in.vertex_position);
  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * light_color;

  // Resulting lighting
  vec3 result = (ambient + diffuse) * fs_in.vertex_color.rgb;
  out_color = vec4(result, 1.0);
}
