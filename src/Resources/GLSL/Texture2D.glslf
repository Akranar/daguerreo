
#version 120

in vec2 texcoord;
varying out vec4 color;
uniform sampler2D texture;

void main()
{
    color = texture2D(texture, texcoord);
    return;
}
