
#version 120

attribute vec4 POSITION0;

uniform mat4 ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    return;
}
