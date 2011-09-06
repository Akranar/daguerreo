#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../DescShaderProgram.h"

#include <iostream>

#include <d3dx10.h>
#include "RendererBasePimpl.h"

//*************************************
//	ShaderProgram
//*************************************

#include "ShaderProgramPimpl.h"
#include "ShaderReflectionPimpl.h"

#include <string>
#include <vector>
#include <algorithm>


void RendererBase::CreateResource(const DescShaderProgram & desc, ShaderProgram * resource)
{
	
	ID3D10Blob * vs_blob;
	ID3D10Blob * ps_blob;
	ID3D10Blob * errors;

	D3DX10CompileFromMemory(desc.GetProgramString(0),desc.GetProgramStringLength(0), "Unused", NULL, NULL, "main", "vs_4_0", 0, 0, NULL, &vs_blob, &errors, NULL);
		
	if (errors)
	{
		std::cout << (char *) errors->GetBufferPointer() << std::endl;
		errors->Release();
	}

	D3DX10CompileFromMemory(desc.GetProgramString(1),desc.GetProgramStringLength(1),"Unused", NULL, NULL, "main", "ps_4_0", 0, 0, NULL, &ps_blob, &errors, NULL);
	if (errors)
	{
		std::cout << (char *) errors->GetBufferPointer() << std::endl;
		errors->Release();
	}

	ShaderProgramPimpl * resource_pimpl = new ShaderProgramPimpl();

	bool vs_failed = FAILED(pimpl->d3d_device->CreateVertexShader((DWORD*) vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &resource_pimpl->vertex_shader));
	bool ps_failed = FAILED(pimpl->d3d_device->CreatePixelShader((DWORD*) ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), &resource_pimpl->pixel_shader));

	if (vs_failed || ps_failed)
	{
		std::cout << "HLSL Shader Creation Failed" << std::endl;
	}
	//Note: About ID3D10ShaderReflection: http://forums.create.msdn.com/forums/t/21239.aspx
	D3D10ReflectShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &resource_pimpl->vs_reflect);
	D3D10ReflectShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), &resource_pimpl->ps_reflect);

	D3D10GetInputSignatureBlob(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &resource_pimpl->vertex_signature);

	resource->PRIVATE = (void *) resource_pimpl;

	D3D10_SHADER_DESC shader_desc;
	resource_pimpl->vs_reflect->GetDesc(&shader_desc);

	unsigned int input_count = shader_desc.InputParameters;

	DescShaderSignature sig;
	sig.signature_entries.Set(input_count);

	for (unsigned int input_index = 0; input_index < input_count; ++input_index)
	{
		D3D10_SIGNATURE_PARAMETER_DESC input_signature;
		resource_pimpl->vs_reflect->GetInputParameterDesc(input_index, &input_signature);

		unsigned int component[4];
		component[0] = (input_signature.Mask & 0x01);
		component[1] = (input_signature.Mask & 0x02) >> 1;
		component[2] = (input_signature.Mask & 0x04) >> 2;
		component[3] = (input_signature.Mask & 0x08) >> 3;

		unsigned int num_components_used = 0;
		for (unsigned int i = 0; i < 4; ++i)
		{
			num_components_used += component[i];
		}
		sig.signature_entries[input_index].name = input_signature.SemanticName;
		sig.signature_entries[input_index].type = DT_FLOAT; //FIX
		sig.signature_entries[input_index].channels = num_components_used;
		sig.signature_entries[input_index].usage_index = input_signature.SemanticIndex;
	}
	//pimpl->d3d_device->CreateBlob( //D3D10CreateBlob
	DescShaderSignature & inserted_signature_desc = GetShaderSignatureHash(sig, resource->signature_hash);
	if (!inserted_signature_desc.PRIVATE)
	{
		//sigged.PRIVATE = (void *) resource_pimpl->vertex_signature;
		D3D10GetInputSignatureBlob(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), (ID3D10Blob **) &inserted_signature_desc.PRIVATE );
	}
	resource->signature_desc = &inserted_signature_desc;
	

	vs_blob->Release();
	ps_blob->Release();


	std::vector<std::pair<std::string, unsigned int> > uniform_names;
	std::vector<UniformParam> uniforms;

	std::vector<std::pair<std::string, unsigned int> > sampler_names;
	std::vector<SamplerParam> samplers;
	std::vector<unsigned int> samplers_domains;
	std::vector<unsigned int> samplers_bindpoints;

	std::vector<UniformBlock> uniform_blocks;
	std::vector<unsigned int> uniform_block_domains;
	std::vector<unsigned int> uniform_block_bindpoints;


	//**************************************
	//	VERTEX UNIFORMS
	//**************************************
	unsigned int vertex_cbuffer_count = shader_desc.ConstantBuffers;
	for (unsigned int cbuffer_index = 0; cbuffer_index < vertex_cbuffer_count; ++cbuffer_index)
	{
		ID3D10ShaderReflectionConstantBuffer * cbuff = resource_pimpl->vs_reflect->GetConstantBufferByIndex(cbuffer_index);

		D3D10_SHADER_BUFFER_DESC shader_buff_desc;
		cbuff->GetDesc(&shader_buff_desc);

		
		unsigned int variable_count = shader_buff_desc.Variables;
		for (unsigned int var_index = 0; var_index < variable_count; ++var_index)
		{

			ID3D10ShaderReflectionVariable * shader_var = cbuff->GetVariableByIndex(var_index);
			
			D3D10_SHADER_VARIABLE_DESC shader_var_desc;
			shader_var->GetDesc(&shader_var_desc);
			ID3D10ShaderReflectionType * type = shader_var->GetType();

			D3D10_SHADER_TYPE_DESC var_type_desc;
			type->GetDesc(&var_type_desc);

			uniform_names.push_back(std::make_pair(shader_var_desc.Name, uniforms.size()));
			uniforms.push_back(UniformParam());
			uniforms.back().array_length = var_type_desc.Elements;
			uniforms.back().uniform_buffer_index = uniform_blocks.size();
			uniforms.back().uniform_buffer_matrix_stride = 0;
			uniforms.back().uniform_buffer_array_stride = 0;
			uniforms.back().uniform_buffer_offset = shader_var_desc.StartOffset;
			uniforms.back().type = DT_FLOAT;
			uniforms.back().rows = var_type_desc.Rows;
			uniforms.back().columns = var_type_desc.Columns;
			switch (var_type_desc.Class)
			{
			case D3D10_SVC_MATRIX_ROWS:
				uniforms.back().row_major = true;
				break;
			case D3D10_SVC_MATRIX_COLUMNS:
				uniforms.back().row_major = false;
				break;
			default:
				uniforms.back().row_major = false;
				break;

			}
		}
		uniform_blocks.push_back(UniformBlock());
		uniform_block_domains.push_back(DOMAIN_VERTEX);
		uniform_block_bindpoints.push_back(cbuffer_index);
		uniform_blocks.back().name = shader_buff_desc.Name;
		uniform_blocks.back().byte_count = shader_buff_desc.Size;
		uniform_blocks.back().uniform_indices.Set(shader_buff_desc.Variables);
	}
	//**************************************
	//	VERTEX SAMPLERS
	//**************************************
	unsigned int vertex_boundresource_count = shader_desc.BoundResources;
	for (unsigned int boundresource_index = 0; boundresource_index < vertex_boundresource_count; ++boundresource_index)
	{
		D3D10_SHADER_INPUT_BIND_DESC input_bind_desc;
		resource_pimpl->vs_reflect->GetResourceBindingDesc(boundresource_index, &input_bind_desc);

		if(input_bind_desc.Type == D3D10_SIT_TEXTURE)
		{
			const char * name = input_bind_desc.Name;
			unsigned int sampler_type;
			unsigned int bind_point = input_bind_desc.BindPoint;
			switch(input_bind_desc.Dimension)
			{
				case D3D10_SRV_DIMENSION_UNKNOWN:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_BUFFER:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE1D:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE1DARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2D:
					{
						sampler_type = ST_SAMPLER_2D;
						
					}
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DMS:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
					case D3D10_SRV_DIMENSION_TEXTURE3D:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURECUBE:
					{
						sampler_type = ST_SAMPLER_CUBE;
					}
					break;
				default:
					break;
			}
			sampler_names.push_back(std::make_pair(name, samplers.size()));
			samplers.push_back(SamplerParam());
			samplers.back().type = sampler_type;
			samplers_bindpoints.push_back(bind_point);
			samplers_domains.push_back(DOMAIN_VERTEX);
		}
	}
	//**************************************
	//	FRAGMENT UNIFORMS
	//**************************************
	resource_pimpl->ps_reflect->GetDesc(&shader_desc);
	unsigned int fragment_cbuffer_count = shader_desc.ConstantBuffers;
	for (unsigned int cbuffer_index = 0; cbuffer_index < fragment_cbuffer_count; ++cbuffer_index)
	{
		ID3D10ShaderReflectionConstantBuffer * cbuff = resource_pimpl->ps_reflect->GetConstantBufferByIndex(cbuffer_index);

		D3D10_SHADER_BUFFER_DESC shader_buff_desc;
		cbuff->GetDesc(&shader_buff_desc);

		
		unsigned int variable_count = shader_buff_desc.Variables;
		for (unsigned int var_index = 0; var_index < variable_count; ++var_index)
		{

			ID3D10ShaderReflectionVariable * shader_var = cbuff->GetVariableByIndex(var_index);
			
			D3D10_SHADER_VARIABLE_DESC shader_var_desc;
			shader_var->GetDesc(&shader_var_desc);
			ID3D10ShaderReflectionType * type = shader_var->GetType();

			D3D10_SHADER_TYPE_DESC var_type_desc;
			type->GetDesc(&var_type_desc);

			uniform_names.push_back(std::make_pair(shader_var_desc.Name, uniforms.size()));
			uniforms.push_back(UniformParam());
			uniforms.back().array_length = var_type_desc.Elements;
			uniforms.back().uniform_buffer_index = uniform_blocks.size();
			uniforms.back().uniform_buffer_matrix_stride = 0;
			uniforms.back().uniform_buffer_array_stride = 0;
			uniforms.back().uniform_buffer_offset = shader_var_desc.StartOffset;
			uniforms.back().type = DT_FLOAT;
			uniforms.back().rows = var_type_desc.Rows;
			uniforms.back().columns = var_type_desc.Columns;
			switch (var_type_desc.Class)
			{
			case D3D10_SVC_MATRIX_ROWS:
				uniforms.back().row_major = true;
				break;
			case D3D10_SVC_MATRIX_COLUMNS:
				uniforms.back().row_major = false;
				break;
			default:
				uniforms.back().row_major = false;
				break;

			}
		}
		uniform_blocks.push_back(UniformBlock());
		uniform_block_domains.push_back(DOMAIN_FRAGMENT);
		uniform_block_bindpoints.push_back(cbuffer_index);
		uniform_blocks.back().name = shader_buff_desc.Name;
		uniform_blocks.back().byte_count = shader_buff_desc.Size;
		uniform_blocks.back().uniform_indices.Set(shader_buff_desc.Variables);
	}
	//**************************************
	//	FRAGMENT SAMPLERS
	//**************************************
	unsigned int fragment_boundresource_count = shader_desc.BoundResources;
	for (unsigned int boundresource_index = 0; boundresource_index < fragment_boundresource_count; ++boundresource_index)
	{
		D3D10_SHADER_INPUT_BIND_DESC input_bind_desc;
		resource_pimpl->ps_reflect->GetResourceBindingDesc(boundresource_index, &input_bind_desc);

		if(input_bind_desc.Type == D3D10_SIT_TEXTURE)
		{
			const char * name = input_bind_desc.Name;
			unsigned int sampler_type;
			unsigned int bind_point = input_bind_desc.BindPoint;
			switch(input_bind_desc.Dimension)
			{
				case D3D10_SRV_DIMENSION_UNKNOWN:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_BUFFER:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE1D:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE1DARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2D:
					{
						sampler_type = ST_SAMPLER_2D;
						
					}
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DMS:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY:
					throw Exception("Texture Type Not Supported.");
					break;
					case D3D10_SRV_DIMENSION_TEXTURE3D:
					throw Exception("Texture Type Not Supported.");
					break;
				case D3D10_SRV_DIMENSION_TEXTURECUBE:
					{
						sampler_type = ST_SAMPLER_CUBE;
					}
					break;
				default:
					break;
			}
			sampler_names.push_back(std::make_pair(name, samplers.size()));
			samplers.push_back(SamplerParam());
			samplers.back().type = sampler_type;
			samplers_bindpoints.push_back(bind_point);
			samplers_domains.push_back(DOMAIN_FRAGMENT);
		}
	}



	std::sort(uniform_names.begin(), uniform_names.end());
	std::sort(sampler_names.begin(), sampler_names.end());

	resource->uniform_blocks.Set(vertex_cbuffer_count + fragment_cbuffer_count);
	resource_pimpl->block_domains.Set(resource->uniform_blocks.GetSize());
	resource_pimpl->block_bind_points.Set(resource->uniform_blocks.GetSize());

	for (unsigned int i = 0; i < uniform_blocks.size(); ++i)
	{
		resource->uniform_blocks[i] = uniform_blocks[i];
		resource_pimpl->block_domains[i] = uniform_block_domains[i];
		resource_pimpl->block_bind_points[i] = uniform_block_bindpoints[i];
	}

	resource->uniforms.Set(uniform_names.size());
	std::vector<unsigned int> block_param_count(resource->uniform_blocks.GetSize(), 0); //Keep track of how many parameters each block has.
	for (unsigned int i = 0; i < uniform_names.size(); ++i)
	{
		//Set uniform info in ShaderProgram.uniforms.
		int index = uniform_names[i].second;
		resource->uniforms[i] = uniforms[index];
		resource->uniforms[i].name = uniform_names[i].first;

		//Put index in the parameter's block in ShaderProgram.uniform_blocks.
		int uniform_block_index = uniforms[index].uniform_buffer_index;
		resource->uniform_blocks[uniform_block_index].uniform_indices[block_param_count[uniform_block_index]] = i;
		block_param_count[uniform_block_index] += 1;
	}
	resource->samplers.Set(sampler_names.size());
	resource_pimpl->sampler_domains.Set(sampler_names.size());
	resource_pimpl->sampler_bind_points.Set(sampler_names.size());
	for (unsigned int i = 0; i < sampler_names.size(); ++i)
	{
		int index = sampler_names[i].second;
		resource->samplers[i].name = sampler_names[i].first;
		resource->samplers[i].type = samplers[index].type;
		resource_pimpl->sampler_bind_points[i] = samplers_bindpoints[index];
		resource_pimpl->sampler_domains[i] = samplers_domains[index];
	}
	
	resource_pimpl->vs_reflect->Release();
	resource_pimpl->ps_reflect->Release();
}

void RendererBase::Delete(ShaderProgram * resource)
{
	ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	resource_pimpl->vertex_shader->Release();
	resource_pimpl->pixel_shader->Release();
	
	delete resource_pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::BindShaderProgram(ShaderProgram * resource)
{
	if (resource)
	{
		ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
		pimpl->d3d_device->VSSetShader(resource_pimpl->vertex_shader);
		pimpl->d3d_device->PSSetShader(resource_pimpl->pixel_shader);
	}
	else
	{
		pimpl->d3d_device->VSSetShader(0);
		pimpl->d3d_device->PSSetShader(0);
	}
}