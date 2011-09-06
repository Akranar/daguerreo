// glslv output by Cg compiler
// cgc version 3.0.0016, build date Feb 11 2011
// command line args: -profile glslv
// source file: TextureCube.cg
//vendor NVIDIA Corporation
//version 3.0.0.16
//profile glslv
//program mainV
//semantic mainV.ModelViewProjMatrix
//var float4x4 ModelViewProjMatrix :  : _ModelViewProjMatrix1[0], 4 : 1 : 1
//var float4 IN.position : $vin.POSITION : ATTR0 : 0 : 1
//var float4 mainV.position : $vout.POSITION : HPOS : -1 : 1
//var float3 mainV.texcoord0 : $vout.TEXCOORD0 : TEX0 : -1 : 1

in vec4 POSITION0;
varying out vec3 model_position;
uniform mat4 ModelViewProjMatrix;

void main()
{
    gl_Position = POSITION0 * ModelViewProjMatrix;
    model_position = POSITION0.xyz;
    return;
}
