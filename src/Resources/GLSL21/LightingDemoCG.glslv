// glslv output by Cg compiler
// cgc version 3.0.0016, build date Feb 11 2011
// command line args: -profile glslv -profileopts version=150
// source file: LightingAndTexture.cg
//vendor NVIDIA Corporation
//version 3.0.0.16
//profile glslv
//program MainV
//semantic MainV.ModelViewProjMatrix
//semantic MainV.ModelMatrix
//semantic global_ambient
//semantic light_color
//semantic light_position
//semantic eye_position
//semantic Ke
//semantic Ka
//semantic Kd
//semantic Ks
//semantic texture
//var float4x4 ModelViewProjMatrix :  : ModelViewProjMatrix[0], 4 : 7 : 1
//var float4x4 ModelMatrix :  : ModelMatrix[0], 4 : 8 : 1
//var float4 position : $vin.POSITION : $POSITION : 0 : 1
//var float3 normal : $vin.NORMAL : $NORMAL : 1 : 1
//var float2 texcoord : $vin.TEXCOORD0 : $TEXCOORD0 : 2 : 1
//var float4 out_position : $vout.POSITION : HPOS : 3 : 1
//var float3 world_position : $vout.TEXCOORD0 : $TEX0 : 4 : 1
//var float3 out_normal : $vout.TEXCOORD1 : $TEX1 : 5 : 1
//var float2 out_texcoord : $vout.TEXCOORD2 : $TEX2 : 6 : 1

#version 150

vec4 _out_position1;
uniform vec4 ModelViewProjMatrix[4];
uniform vec4 ModelMatrix[4];
vec4 _r0014;
vec4 _r0024;
vec3 _r0034;
in vec4 POSITION;
in vec4 NORMAL;
in vec4 TEXCOORD0;
out vec4 TEX0;
out vec4 TEX1;
out vec4 TEX2;
vec3 _TMP41;
vec3 _TMP42;
vec3 _TMP43;

 // main procedure, the original name was MainV
void main()
{

    vec3 _world_position;
    vec3 _out_normal;
    vec2 _out_texcoord;

    _r0014.x = dot(ModelViewProjMatrix[0], POSITION);
    _r0014.y = dot(ModelViewProjMatrix[1], POSITION);
    _r0014.z = dot(ModelViewProjMatrix[2], POSITION);
    _r0014.w = dot(ModelViewProjMatrix[3], POSITION);
    _out_position1 = _r0014;
    _r0024.x = dot(ModelMatrix[0], POSITION);
    _r0024.y = dot(ModelMatrix[1], POSITION);
    _r0024.z = dot(ModelMatrix[2], POSITION);
    _world_position = _r0024.xyz;
    _TMP41.x = ModelMatrix[0].x;
    _TMP41.y = ModelMatrix[0].y;
    _TMP41.z = ModelMatrix[0].z;
    _r0034.x = dot(_TMP41, NORMAL.xyz);
    _TMP42.x = ModelMatrix[1].x;
    _TMP42.y = ModelMatrix[1].y;
    _TMP42.z = ModelMatrix[1].z;
    _r0034.y = dot(_TMP42, NORMAL.xyz);
    _TMP43.x = ModelMatrix[2].x;
    _TMP43.y = ModelMatrix[2].y;
    _TMP43.z = ModelMatrix[2].z;
    _r0034.z = dot(_TMP43, NORMAL.xyz);
    _out_normal = _r0034;
    _out_texcoord = TEXCOORD0.xy;
    gl_Position = _r0014;
    TEX0.xyz = _r0024.xyz;
    TEX1.xyz = _r0034;
    TEX2.xy = TEXCOORD0.xy;
} // main end
