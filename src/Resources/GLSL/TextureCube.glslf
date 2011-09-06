// glslf output by Cg compiler
// cgc version 3.0.0016, build date Feb 11 2011
// command line args: -profile glslf
// source file: TextureCube.cg
//vendor NVIDIA Corporation
//version 3.0.0.16
//profile glslf
//program mainF
//semantic mainF.image
//var samplerCUBE image :  : _image1 : 1 : 1
//var float3 IN.texcoord : $vin.TEXCOORD0 : TEXCOORD0 : 0 : 1
//var float4 mainF.color : $vout.COLOR : COL : -1 : 1

in vec3 model_position;
varying out vec4 color;
uniform samplerCube image;

void main()
{
    color = textureCube(image, model_position);
    return;
} 
