
#version 120

in vec4 POSITION0;
in vec2 TEXCOORD0;

varying out vec2 texcoord;

uniform mat4  ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    texcoord = TEXCOORD0;
    return;
}
