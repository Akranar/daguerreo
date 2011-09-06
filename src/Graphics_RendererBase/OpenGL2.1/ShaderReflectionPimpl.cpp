#include "ShaderReflectionPimpl.h"
#include <iostream>

void ShaderReflectionPimpl::PrintUniformParams(GLuint program)
{
	char uniform_name[1000];
	GLsizei name_length;
	GLint uniform_size;
	GLenum uniform_type;
	GLint uniform_count;

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

	for (int i = 0; i < uniform_count; ++i)
	{
		glGetActiveUniform(program, i, 1000, &name_length, &uniform_size, &uniform_type, uniform_name);

		std::cout << "Uniform: " << uniform_name << std::endl;
		std::cout << "	Size: " << uniform_size << std::endl;
		std::cout << "	Index: " << i << std::endl;
		std::cout << "	Location: " << glGetUniformLocation(program, uniform_name) << std::endl;
		//std::cout << "	Type: " << uniform_type << std::endl;
		//FLOAT, INT, BOOL, SAMPLER

		switch (uniform_type)
		{
			case GL_FLOAT:
				std::cout << "	Type: GL_FLOAT" << std::endl;
				break;
			case GL_FLOAT_VEC2:
				std::cout << "	Type: GL_FLOAT_VEC2" << std::endl;
				break;
			case GL_FLOAT_VEC3:
				std::cout << "	Type: GL_FLOAT_VEC3" << std::endl;
				break;
			case GL_FLOAT_VEC4: 
				std::cout << "	Type: GL_FLOAT_VEC4" << std::endl;
				break;
			case GL_FLOAT_MAT2: 
				std::cout << "	Type: GL_FLOAT_MAT2" << std::endl;
				break;
			case GL_FLOAT_MAT3: 
				std::cout << "	Type: GL_FLOAT_MAT3" << std::endl;
				break;
			case GL_FLOAT_MAT4: 
				std::cout << "	Type: GL_FLOAT_MAT4" << std::endl;
				break;
			case GL_FLOAT_MAT2x3: 
				std::cout << "	Type: GL_FLOAT_MAT2x3" << std::endl;
				break;
			case GL_FLOAT_MAT2x4: 
				std::cout << "	Type: GL_FLOAT_MAT2x4" << std::endl;
				break;
			case GL_FLOAT_MAT3x2: 
				std::cout << "	Type: GL_FLOAT_MAT3x2" << std::endl;
				break;
			case GL_FLOAT_MAT3x4: 
				std::cout << "	Type: GL_FLOAT_MAT3x4" << std::endl;
				break;
			case GL_FLOAT_MAT4x2: 
				std::cout << "	Type: GL_FLOAT_MAT4x2" << std::endl;
				break;
			case GL_FLOAT_MAT4x3:
				std::cout << "	Type: GL_FLOAT_MAT4x3" << std::endl;
				break;
			case GL_SAMPLER_1D:
				std::cout << "	Type: GL_SAMPLER_1D" << std::endl;
				break;
			case GL_SAMPLER_2D:
				std::cout << "	Type: GL_SAMPLER_2D" << std::endl;
				break;
			case GL_SAMPLER_3D:
				std::cout << "	Type: GL_SAMPLER_3D" << std::endl;
				break;
			case GL_SAMPLER_CUBE:
				std::cout << "	Type: GL_SAMPLER_CUBE" << std::endl;
				break;
			case GL_SAMPLER_1D_SHADOW:
				std::cout << "	Type: GL_SAMPLER_1D_SHADOW" << std::endl;
				break;
			case GL_SAMPLER_2D_SHADOW:
				std::cout << "	Type: GL_SAMPLER_2D_SHADOW" << std::endl;
				break;
		}
	}
}

void ShaderReflectionPimpl::PrintAttributes(GLuint program)
{
	char attrib_name[1000];
	GLsizei name_length;
	GLint attrib_size;
	GLenum attrib_type;
	GLint attrib_count;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrib_count);

	for (int i = 0; i < attrib_count; ++i)
	{
		glGetActiveAttrib(program, i, 1000, &name_length, &attrib_size, &attrib_type, attrib_name);

		std::cout << "Attrib: " << attrib_name << std::endl;
		std::cout << "	Size: " << attrib_size << std::endl;
		std::cout << "	Index: " << i << std::endl;
		std::cout << "	Location: " << glGetAttribLocation(program, attrib_name) << std::endl;
		switch (attrib_type)
		{
			case GL_FLOAT:
				std::cout << "	Type: GL_FLOAT" << std::endl;
				break;
			case GL_FLOAT_VEC2:
				std::cout << "	Type: GL_FLOAT_VEC2" << std::endl;
				break;
			case GL_FLOAT_VEC3:
				std::cout << "	Type: GL_FLOAT_VEC3" << std::endl;
				break;
			case GL_FLOAT_VEC4: 
				std::cout << "	Type: GL_FLOAT_VEC4" << std::endl;
				break;
			case GL_FLOAT_MAT2: 
				std::cout << "	Type: GL_FLOAT_MAT2" << std::endl;
				break;
			case GL_FLOAT_MAT3: 
				std::cout << "	Type: GL_FLOAT_MAT3" << std::endl;
				break;
			case GL_FLOAT_MAT4: 
				std::cout << "	Type: GL_FLOAT_MAT4" << std::endl;
				break;
			case GL_FLOAT_MAT2x3: 
				std::cout << "	Type: GL_FLOAT_MAT2x3" << std::endl;
				break;
			case GL_FLOAT_MAT2x4: 
				std::cout << "	Type: GL_FLOAT_MAT2x4" << std::endl;
				break;
			case GL_FLOAT_MAT3x2: 
				std::cout << "	Type: GL_FLOAT_MAT3x2" << std::endl;
				break;
			case GL_FLOAT_MAT3x4: 
				std::cout << "	Type: GL_FLOAT_MAT3x4" << std::endl;
				break;
			case GL_FLOAT_MAT4x2: 
				std::cout << "	Type: GL_FLOAT_MAT4x2" << std::endl;
				break;
			case GL_FLOAT_MAT4x3:
				std::cout << "	Type: GL_FLOAT_MAT4x3" << std::endl;
				break;
		}
	}
}

void ShaderReflectionPimpl::PrintUniformBuffers(GLuint program)
{
	if (glGetActiveUniformBlockiv)
	{
		char uniformblock_name[1000];
		GLint name_length;
		GLint block_binding;
		GLint block_data_size;
		GLint block_active_uniforms;
		GLint uniformblock_count;
		GLint referenced_by_vertex_shader;
		GLint referenced_by_fragment_shader;

		glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &uniformblock_count);
	
		for (int i = 0; i < uniformblock_count; ++i)
		{
			glGetActiveUniformBlockName(program, i, 1000, &name_length, uniformblock_name);

			GLint params;
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, &block_binding);
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &block_data_size);
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &params);
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &block_active_uniforms);
			std::vector<GLuint> uniform_indices(block_active_uniforms, 0);
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<GLint*>(&uniform_indices[0]));
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &referenced_by_vertex_shader);
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, &referenced_by_fragment_shader);

			std::vector<GLuint> uniform_offsets(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_OFFSET, reinterpret_cast<GLint*>(&uniform_offsets[0]));
			std::vector<GLuint> uniform_arraystrides(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_ARRAY_STRIDE, reinterpret_cast<GLint*>(&uniform_arraystrides[0]));
			std::vector<GLuint> uniform_matrixstrides(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_MATRIX_STRIDE, reinterpret_cast<GLint*>(&uniform_matrixstrides[0]));
			std::vector<GLuint> uniform_rowmajor(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_IS_ROW_MAJOR, reinterpret_cast<GLint*>(&uniform_rowmajor[0]));

			std::cout << "UniformBlock: " << uniformblock_name << "[" << i << "]" << std::endl;
			std::cout << "		Size: " << block_data_size << std::endl;
			std::cout << "		UniformCount: " << block_active_uniforms << std::endl;
			std::cout << "		Referenced VS: " << referenced_by_vertex_shader << std::endl;
			std::cout << "		Referenced FS: " << referenced_by_fragment_shader << std::endl;
			for (unsigned int p = 0; p < uniform_indices.size(); ++p)
			{
				std::cout << "	Param: " << uniform_indices[p] << std::endl;
				std::cout << "		Offset: " << uniform_offsets[p] << std::endl;
				std::cout << "		ArrayStrides: " << uniform_arraystrides[p] << std::endl;
				std::cout << "		MatrixStrides: " << uniform_matrixstrides[p] << std::endl;
				std::cout << "		RowMajor: " << uniform_rowmajor[p] << std::endl;
			}
			//std::cout << "	Location: " << glGetUniformLocation(pimpl->combined, uniformblock_name) << std::endl;
		}
	}
}