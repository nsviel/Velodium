#version 330

in vec3 vertex_in;
in vec2 texCoord_in;
out vec2 texCoord_v;

void main()
{
    texCoord_v = texCoord_in;
    gl_Position =  vec4(vertex_in, 1.0);
}
