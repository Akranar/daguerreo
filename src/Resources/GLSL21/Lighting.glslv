
#version 120

attribute vec4 POSITION0;
attribute vec3 NORMAL0;
attribute vec2 TEXCOORD0;

varying vec3 position;
varying vec3 normal;
varying vec2 texcoord;

uniform mat4  ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    position = POSITION0.xyz;
    normal = NORMAL0;
    texcoord = TEXCOORD0;
    return;
}
