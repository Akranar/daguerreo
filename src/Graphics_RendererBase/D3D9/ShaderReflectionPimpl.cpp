#include "ShaderReflectionPimpl.h"
#include <iostream>
#include <vector>

void ShaderReflectionPimpl::PrintUniforms(ID3DXConstantTable * table)
{

	std::vector<D3DXHANDLE> constants;

	unsigned int i = 0;
	for (unsigned int i = 0;;++i)
	{
		D3DXHANDLE constant = table->GetConstant(NULL, i);
		if (constant)
		{
			constants.push_back(constant);
		}
		else
		{
			break;
		}
	}

	std::vector<D3DXCONSTANT_DESC> descs(constants.size());
	UINT size = descs.size();
	for (unsigned int i = 0; i < constants.size(); ++i)
	{
		table->GetConstantDesc(constants[i], &descs[i], &size);
	}

	unsigned int max_register = 0;
	unsigned int max_register_index = 0;
	for (unsigned int i = 0; i < constants.size(); ++i)
	{
		if (max_register < descs[i].RegisterIndex)
		{
			max_register = descs[i].RegisterIndex;
			max_register_index = i;
		}
	}

	int register_count = max_register + descs[max_register_index].RegisterCount + 1;//0 indexed.

	int x = 0;
	
	//std::cout << table->GetBufferPointer() << std::endl;
}