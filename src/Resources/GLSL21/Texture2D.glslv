
#version 120

attribute vec4 POSITION0;
attribute vec2 TEXCOORD0;

varying vec2 texcoord;

uniform mat4  ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    texcoord = TEXCOORD0;
    return;
}
