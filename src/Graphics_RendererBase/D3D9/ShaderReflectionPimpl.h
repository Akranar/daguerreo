#ifndef _SHADER_REFLECTION_PIMPL_H_
#define _SHADER_REFLECTION_PIMPL_H_

#include <d3d9.h>
#include <d3dx9.h>

class ShaderReflectionPimpl
{
public:
	void PrintUniforms(ID3DXConstantTable * table);
};

#endif