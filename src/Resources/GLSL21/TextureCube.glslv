
attribute vec4 POSITION0;
varying vec3 model_position;
uniform mat4 ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    model_position = POSITION0.xyz;
    return;
}
