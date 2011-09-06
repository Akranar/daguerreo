#include "ShaderReflectionPimpl.h"
#include <iostream>

void ShaderReflectionPimpl::PrintReflection(ID3D10ShaderReflection * reflection)
{
	std::cout << "ShaderReflection: " << std::endl;
	D3D10_SHADER_DESC shader_desc;
	reflection->GetDesc(&shader_desc);


	unsigned int input_count = shader_desc.InputParameters;
	std::cout << "	InputCount: " << input_count << std::endl;

	for (unsigned int input_index = 0; input_index < input_count; ++input_index)
	{
		D3D10_SIGNATURE_PARAMETER_DESC input_signature;
		reflection->GetInputParameterDesc(input_index, &input_signature);

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

		std::cout << "		Input: " << input_signature.SemanticName << " | ";
		std::cout << input_signature.SemanticIndex << " | ";
		std::cout << input_signature.Register << " | ";
		std::cout << component[3] << component[2] << component[1] << component[0] << " | " << num_components_used << std::endl;
	}

	unsigned int cbuffer_count = shader_desc.ConstantBuffers;
	std::cout << "	CBufferCount: " << cbuffer_count << std::endl;

	for (unsigned int cbuffer_index = 0; cbuffer_index < cbuffer_count; ++cbuffer_index)
	{
		ID3D10ShaderReflectionConstantBuffer * cbuff = reflection->GetConstantBufferByIndex(cbuffer_index);

		D3D10_SHADER_BUFFER_DESC shader_buff_desc;
		cbuff->GetDesc(&shader_buff_desc);

		std::cout << "		CBuffer: " << shader_buff_desc.Name << " | " << shader_buff_desc.Size << " | " << shader_buff_desc.Type << std::endl;

		unsigned int variable_count = shader_buff_desc.Variables;

		for (unsigned int var_index = 0; var_index < variable_count; ++var_index)
		{

			ID3D10ShaderReflectionVariable * shader_var = cbuff->GetVariableByIndex(var_index);
			

			D3D10_SHADER_VARIABLE_DESC shader_var_desc;
			shader_var->GetDesc(&shader_var_desc);
			ID3D10ShaderReflectionType * type = shader_var->GetType();

			D3D10_SHADER_TYPE_DESC var_type_desc;
			type->GetDesc(&var_type_desc);

			std::cout << "			Var: " << shader_var_desc.Name << std::endl;
			std::cout << "				Size: " << shader_var_desc.Size << std::endl;
			std::cout << "				Offset: " << shader_var_desc.StartOffset << std::endl;
			std::cout << "				Class: " << var_type_desc.Class << std::endl;
			std::cout << "				Type: " << var_type_desc.Type << std::endl;
			std::cout << "				Rows: " << var_type_desc.Rows << std::endl;
			std::cout << "				Columns: " << var_type_desc.Columns << std::endl;
			switch (var_type_desc.Class)
			{
			case D3D10_SVC_MATRIX_ROWS:
				std::cout << "				Order: Row-Major" << std::endl;
				break;
			case D3D10_SVC_MATRIX_COLUMNS:
				std::cout << "				Order: Column-Major" << std::endl;
				break;
			default:
				std::cout << "				Order: Column-Major" << std::endl;
				break;

			}
		}
	}

	unsigned int boundresource_count = shader_desc.BoundResources;
	std::cout << "	BoundResources: " << boundresource_count << std::endl;

	for (unsigned int boundresource_index = 0; boundresource_index < boundresource_count; ++boundresource_index)
	{
		D3D10_SHADER_INPUT_BIND_DESC input_bind_desc;
		reflection->GetResourceBindingDesc(boundresource_index, &input_bind_desc);

		std::cout << "		Name: " << input_bind_desc.Name << std::endl;
		std::cout << "			BindPoint: " << input_bind_desc.BindPoint << std::endl;

		switch(input_bind_desc.Dimension)
		{
			case D3D10_SRV_DIMENSION_UNKNOWN:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_UNKNOWN" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_BUFFER:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_BUFFER" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE1D:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE1D" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE1DARRAY:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE1DARRAY" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE2D:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE2D" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE2DARRAY:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE2DARRAY" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE2DMS:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE2DMS" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY" << std::endl;
				break;
				case D3D10_SRV_DIMENSION_TEXTURE3D:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURE3D" << std::endl;
				break;
			case D3D10_SRV_DIMENSION_TEXTURECUBE:
				std::cout << "			Dimension: D3D10_SRV_DIMENSION_TEXTURECUBE" << std::endl;
				break;
			default:
				break;
		}


		switch(input_bind_desc.Type)
		{
			case D3D10_SIT_CBUFFER:
				std::cout << "			Type: D3D10_SIT_CBUFFER" << std::endl;
				break;
			case D3D10_SIT_TBUFFER:
				std::cout << "			Type: D3D10_SIT_TBUFFER" << std::endl;
				break;
			case D3D10_SIT_TEXTURE:
				std::cout << "			Type: D3D10_SIT_TEXTURE" << std::endl;
				break;
			case D3D10_SIT_SAMPLER:
				std::cout << "			Type: D3D10_SIT_SAMPLER" << std::endl;
				break;
			default:
				break;
		}
	}
}