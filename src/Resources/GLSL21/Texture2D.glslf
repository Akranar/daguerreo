
#version 120

varying vec2 texcoord;
//varying vec4 color;
uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, texcoord);
    return;
}
