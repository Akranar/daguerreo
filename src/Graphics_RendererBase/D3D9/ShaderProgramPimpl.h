#ifndef _SHADER_PROGRAM_PIMPL_H_
#define _SHADER_PROGRAM_PIMPL_H_

#include <d3dx9.h>
#include <d3d9.h>

#include <Core_FixedArray.h>

class ShaderProgramPimpl
{
public:
	IDirect3DVertexShader9 * vertex_shader;
	IDirect3DPixelShader9 * pixel_shader;
	ID3DXConstantTable * vertex_constant_table;
	ID3DXConstantTable * pixel_constant_table;
	FixedArray<unsigned int> sampler_bind_points;
};

#endif