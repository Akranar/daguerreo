#include "Interface_ShaderProgram.h"

#include <iostream>

void ShaderProgram::PrintUniforms()
{
	for (unsigned int i = 0; i < uniforms.GetSize(); ++i)
	{
		UniformParam & uniform = uniforms[i];
		std::cout << "Uniform: " << uniform.name << std::endl;
		std::cout << "	Type: " << (unsigned int)uniform.type << std::endl;
		std::cout << "	RowMajor: " << (unsigned int)uniform.row_major << std::endl;
		std::cout << "	Rows: " << (unsigned int)uniform.rows << std::endl;
		std::cout << "	Columns: " << (unsigned int)uniform.columns << std::endl;
		std::cout << "	ArrayLength: " << uniform.array_length << std::endl;
		std::cout << "	BufferIndex: " << uniform.uniform_buffer_index << std::endl;
		std::cout << "	BufferOffset: " << uniform.uniform_buffer_offset << std::endl;
		std::cout << "	BufferArrayStride: " << uniform.uniform_buffer_array_stride << std::endl;
		std::cout << "	BufferMatrixStride: " << uniform.uniform_buffer_matrix_stride << std::endl;
	}
}

void ShaderProgram::PrintBlocks()
{
	for (unsigned int i = 0; i < uniform_blocks.GetSize(); ++i)
	{
		std::cout << "Block: " << uniform_blocks[i].name << std::endl;
		std::cout << "	Size: " << uniform_blocks[i].byte_count << std::endl;
		std::cout << "	Uniforms: ";
		for (unsigned int j = 0; j < uniform_blocks[i].uniform_indices.GetSize(); ++j)
		{
			std::cout << uniform_blocks[i].uniform_indices[j] << ", ";
		}
		std::cout << std::endl;
	}
}