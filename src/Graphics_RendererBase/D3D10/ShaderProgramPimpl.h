#ifndef _SHADER_PROGRAM_PIMPL_H_
#define _SHADER_PROGRAM_PIMPL_H_

#include <d3d10.h>
#include <d3dx10.h>

#include <Core_FixedArray.h>

class ShaderDomainPimpl
{
public:
	FixedArray<unsigned int> sampler_bind_points;
};

class ShaderProgramPimpl
{
public:
	ID3D10VertexShader * vertex_shader;
	ID3D10PixelShader * pixel_shader;
	ID3D10ShaderReflection * vs_reflect;
	ID3D10ShaderReflection * ps_reflect;
	ID3D10Blob * vertex_signature;
	FixedArray<unsigned int> block_domains;
	FixedArray<unsigned int> block_bind_points;
	FixedArray<unsigned int> sampler_domains;
	FixedArray<unsigned int> sampler_bind_points;
};

#endif