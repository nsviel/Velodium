#version 330 core

in vec4 color_vertex;

layout(location = 0) out vec4 color_out;


void main()
{
    color_out = color_vertex;
}
