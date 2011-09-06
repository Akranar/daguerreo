
#version 120

in vec4 POSITION0;
in vec3 NORMAL0;
in vec2 TEXCOORD0;

varying out vec3 position;
varying out vec3 normal;
varying out vec2 texcoord;

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
