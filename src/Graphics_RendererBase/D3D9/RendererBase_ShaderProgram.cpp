#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../DescShaderProgram.h"

#include <iostream>
#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"
#include "ShaderReflectionPimpl.h"



#include <iostream>

//*************************************
//	ShaderProgram
//*************************************


void RendererBase::CreateResource(const DescShaderProgram & desc, ShaderProgram * resource)
{
	ShaderProgramPimpl * resource_pimpl = new ShaderProgramPimpl();
	ID3DXBuffer * errors;
	ID3DXBuffer * vertex_buffer;
	ID3DXBuffer * fragment_buffer;
	HRESULT result = D3DXCompileShader(desc.GetProgramString(0), desc.GetProgramStringLength(0), NULL, NULL, "main", "vs_3_0", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &vertex_buffer, &errors, &resource_pimpl->vertex_constant_table);
	if (errors)
	{
		std::cout << (char *) errors->GetBufferPointer() << std::endl;
	}
	result = D3DXCompileShader(desc.GetProgramString(1), desc.GetProgramStringLength(1), NULL, NULL, "main", "ps_3_0", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &fragment_buffer, &errors, &resource_pimpl->pixel_constant_table);
	if (errors)
	{
		std::cout << (char *) errors->GetBufferPointer() << std::endl;
	}
	result = pimpl->d3d_device->CreateVertexShader((DWORD*)vertex_buffer->GetBufferPointer(), &resource_pimpl->vertex_shader);
	result = pimpl->d3d_device->CreatePixelShader((DWORD*)fragment_buffer->GetBufferPointer(), &resource_pimpl->pixel_shader);
	resource->PRIVATE = (void *) resource_pimpl;

	ShaderReflectionPimpl refl;
	refl.PrintUniforms(resource_pimpl->vertex_constant_table);
	refl.PrintUniforms(resource_pimpl->pixel_constant_table);
}

void RendererBase::Delete(ShaderProgram * resource)
{
	ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	resource_pimpl->vertex_shader->Release();
	resource_pimpl->pixel_shader->Release();
	resource_pimpl->vertex_constant_table->Release();
	resource_pimpl->pixel_constant_table->Release();
	delete resource_pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::BindShaderProgram(ShaderProgram * resource)
{
	if (resource)
	{
		ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
		HRESULT result = pimpl->d3d_device->SetVertexShader(resource_pimpl->vertex_shader);
		result = pimpl->d3d_device->SetPixelShader(resource_pimpl->pixel_shader);
		int x = 0;
	}
	else
	{
		pimpl->d3d_device->SetVertexShader(0);
		pimpl->d3d_device->SetPixelShader(0);
	}
}