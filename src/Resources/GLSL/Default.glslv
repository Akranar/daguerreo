
#version 120

in vec4 POSITION0;

uniform mat4 ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    return;
}
