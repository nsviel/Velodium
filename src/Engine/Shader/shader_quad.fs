#version 330

in vec2 texCoord_v;
uniform sampler2D tex0;
out vec4 color_out;

void main()
{
    // Recupération de la couleur du pixel courant
    color_out = texture(tex0, texCoord_v.xy);;
}
