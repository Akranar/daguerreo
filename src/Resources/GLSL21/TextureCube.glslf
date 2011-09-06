
varying vec3 model_position;
//varying vec4 color;
uniform samplerCube image;

void main()
{
    gl_FragColor = textureCube(image, model_position);
    return;
} 
