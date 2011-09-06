
#version 120

in vec4 POSITION0;
in vec3 NORMAL0;
in vec2 TEXCOORD0;

varying out vec3 position;
varying out vec3 normal;
varying out vec2 texcoord;

uniform mat4 ModelViewProjMatrix;
uniform mat4 ModelMatrix;

in vec4 BLENDWEIGHT0;
in vec4 BLENDINDICES0;
uniform mat4 palette[60];

void main()
{
    //mat4 skinmat = BLENDWEIGHT0.x * palette[BLENDINDICES0.x] + 
    //               BLENDWEIGHT0.y * palette[1] + 
    //               BLENDWEIGHT0.z * palette[2] + 
    //               BLENDWEIGHT0.w * palette[3];

    ivec4 int_bone_indices = ivec4(BLENDINDICES0);

    mat4 skinmat = BLENDWEIGHT0.x * palette[int_bone_indices.x]; 
    if (BLENDWEIGHT0.y > 0)
        skinmat += BLENDWEIGHT0.y * palette[int_bone_indices.y];
    if (BLENDWEIGHT0.z > 0)
        skinmat += BLENDWEIGHT0.z * palette[int_bone_indices.z];
    if (BLENDWEIGHT0.w > 0)
        skinmat += BLENDWEIGHT0.w * palette[int_bone_indices.w];

    float total = BLENDWEIGHT0.x + BLENDWEIGHT0.y + BLENDWEIGHT0.z + BLENDWEIGHT0.w;
    if (total < 0.98f || total > 1.01f)
    {
        gl_Position = POSITION0 * skinmat * ModelViewProjMatrix;
        position = vec3(0, 0, 0);
        normal = vec3(0, 0, 0);
        texcoord = vec2(0, 0);
        return;
        
    }
    else
    {
        gl_Position = vec4((POSITION0 * skinmat).xyz, 1.0f) * ModelViewProjMatrix;
        position = (POSITION0 * skinmat * ModelMatrix).xyz;
        normal = NORMAL0 * mat3(skinmat * ModelMatrix);

        //gl_Position = POSITION0 * skinmat * ModelViewProjMatrix;
        //position = (POSITION0 * skinmat * ModelMatrix).xyz;
        //normal = NORMAL0 * mat3(skinmat * ModelMatrix);

        texcoord = TEXCOORD0;
        return;
    }
}
