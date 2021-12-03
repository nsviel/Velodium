#version 330 core

in vec4 color_vertex;
out vec4 color_fragment;

void main()
{
    color_fragment = vec4(color_vertex);
}
