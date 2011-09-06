// glslf output by Cg compiler
// cgc version 3.0.0016, build date Feb 11 2011
// command line args: -profile glslf -profileopts version=150
// source file: LightingAndTexture.cg
//vendor NVIDIA Corporation
//version 3.0.0.16
//profile glslf
//program MainF
//semantic global_ambient
//semantic light_color
//semantic light_position
//semantic eye_position
//semantic Ke
//semantic Ka
//semantic Kd
//semantic Ks
//semantic texture
//var float3 global_ambient :  : global_ambient : -1 : 1
//var float3 light_color :  : light_color : -1 : 1
//var float3 light_position :  : light_position : -1 : 1
//var float3 eye_position :  : eye_position : -1 : 1
//var float3 Ke :  : Ke : -1 : 1
//var float3 Ka :  : Ka : -1 : 1
//var float3 Kd :  : Kd : -1 : 1
//var float4 Ks :  : Ks : -1 : 1
//var sampler2D image :  : image : -1 : 1
//var float3 position : $vin.TEXCOORD0 : $TEX0 : 1 : 1
//var float3 normal : $vin.TEXCOORD1 : $TEX1 : 2 : 1
//var float2 texcoord : $vin.TEXCOORD2 : $TEX2 : 3 : 1
//var float4 color : $vout.COLOR : $COL0 : 4 : 1

#version 150

vec4 _TMP3;
float _TMP2;
float _TMP1;
float _TMP5;
float _TMP4;
float _TMP0;
vec3 _v0022;
vec3 _v0032;
vec3 _v0038;
in vec4 TEX0;
in vec4 TEX1;
in vec4 TEX2;
out vec4 COL0;
uniform vec3 global_ambient;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 Ke;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec4 Ks;
uniform sampler2D image;

 // main procedure, the original name was MainF
void main()
{

    vec4 _color;
    vec3 _N;
    vec3 _ambient;
    vec3 _L;
    float _diffuse_light;
    vec3 _diffuse;
    vec3 _V;
    vec3 _H;
    float _specular_light;
    vec3 _specular;
    vec3 _lighting_color;

    _TMP4 = dot(TEX1.xyz, TEX1.xyz);
    _TMP5 = inversesqrt(_TMP4);
    _N = _TMP5*TEX1.xyz;
    _ambient = Ka*global_ambient;
    _v0022 = light_position - TEX0.xyz;
    _TMP4 = dot(_v0022, _v0022);
    _TMP5 = inversesqrt(_TMP4);
    _L = _TMP5*_v0022;
    _TMP0 = dot(_N, _L);
    _diffuse_light = max(_TMP0, 0.00000000E+000);
    _diffuse = (Kd*light_color)*_diffuse_light;
    _v0032 = eye_position - TEX0.xyz;
    _TMP4 = dot(_v0032, _v0032);
    _TMP5 = inversesqrt(_TMP4);
    _V = _TMP5*_v0032;
    _v0038 = _L + _V;
    _TMP4 = dot(_v0038, _v0038);
    _TMP5 = inversesqrt(_TMP4);
    _H = _TMP5*_v0038;
    _TMP1 = dot(_N, _H);
    _TMP2 = max(_TMP1, 0.00000000E+000);
    _specular_light = pow(_TMP2, Ks.w);
    if (_diffuse_light <= 0.00000000E+000) { // if begin
        _specular_light = 0.00000000E+000;
    } // end if
    _specular = (Ks.xyz*light_color)*_specular_light;
    _TMP3 = texture(image, TEX2.xy);
    _lighting_color = Ke + _ambient + _diffuse;
    _color.xyz = _lighting_color*_TMP3.xyz + _specular;
    _color.w = 1.00000000E+000;
    COL0 = _color;
} // main end
