
#version 120

attribute vec4 POSITION0;
attribute vec3 NORMAL0;
attribute vec2 TEXCOORD0;

varying vec3 position;
varying vec3 normal;
varying vec2 texcoord;

uniform mat4 ModelViewProjMatrix;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    position = (POSITION0 * ModelMatrix).xyz;
    normal = NORMAL0 * mat3(ModelMatrix);
    texcoord = TEXCOORD0;
    return;
}
