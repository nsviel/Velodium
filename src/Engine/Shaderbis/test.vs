#version 330 core

layout(location = 0) in vec3 in_position;
uniform mat4 MVP;

out vec2 UV;

void main()
{
    //Location
    vec4 XYZ = vec4(in_position, 1.0);
    XYZ =  MVP * XYZ;
    gl_Position = XYZ;

    //Color
    UV = (in_position.xy+vec2(1,1))/2.0;
}
