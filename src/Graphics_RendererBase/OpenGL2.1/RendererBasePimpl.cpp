#include "RendererBasePimpl.h"

#include <ctype.h>
#include <cstdlib>

#include "../IncludeAll_Interface.h"
#include "../Enumerants.h"

#include "ShaderProgramPimpl.h"
#include <algorithm>
#include <iostream>

GLuint RendererBasePimpl::GetAvailablePBO()
{
	return pbo_pool[0];
}

void RendererBasePimpl::InputLayoutHelper_AppendZeroIfLastAlpha(std::string & name)
{
	int substr_length = name.length();
	if (isalpha(name[substr_length-1]))
	{
		name.push_back('0');
	}
}

bool RendererBasePimpl::InputLayoutHelper_NameToUsage(const std::string & name, unsigned int & rtn_usage_length, unsigned int & rtn_usage_index)
{
	rtn_usage_length = name.length();
	while (isdigit(name[rtn_usage_length-1]))
	{
		rtn_usage_length -= 1;
	}
	if (rtn_usage_length == name.length())
	{
		rtn_usage_index = 0;
	}
	rtn_usage_index = atoi(name.c_str()+rtn_usage_length);//atoi(name.substr(rtn_usage_length, name.length() - rtn_usage_length).c_str());
	return true;
}

void RendererBasePimpl::GetTypeInfo(GLuint gl_type, unsigned char & type, unsigned char & rows, unsigned char & columns, bool & sampler)
{
	switch (gl_type)
	{
		case GL_FLOAT:
			type = DT_FLOAT;
			rows = 1;
			columns = 1;
			break;
		case GL_FLOAT_VEC2:
			type = DT_FLOAT;
			rows = 1;
			columns = 2;
			break;
		case GL_FLOAT_VEC3:
			type = DT_FLOAT;
			rows = 1;
			columns = 3;
			break;
		case GL_FLOAT_VEC4: 
			type = DT_FLOAT;
			rows = 1;
			columns = 4;
			break;
		case GL_FLOAT_MAT2: 
			type = DT_FLOAT;
			rows = 2;
			columns = 2;
			break;
		case GL_FLOAT_MAT3: 
			type = DT_FLOAT;
			rows = 3;
			columns = 3;
			break;
		case GL_FLOAT_MAT4: 
			type = DT_FLOAT;
			rows = 4;
			columns = 4;
			break;
		case GL_FLOAT_MAT2x3: 
			type = DT_FLOAT;
			rows = 3;
			columns = 2;
			break;
		case GL_FLOAT_MAT2x4: 
			type = DT_FLOAT;
			rows = 4;
			columns = 2;
			break;
		case GL_FLOAT_MAT3x2: 
			type = DT_FLOAT;
			rows = 2;
			columns = 3;
			break;
		case GL_FLOAT_MAT3x4: 
			type = DT_FLOAT;
			rows = 4;
			columns = 3;
			break;
		case GL_FLOAT_MAT4x2: 
			type = DT_FLOAT;
			rows = 2;
			columns = 4;
			break;
		case GL_FLOAT_MAT4x3:
			type = DT_FLOAT;
			rows = 3;
			columns = 4;
			break;
		case GL_SAMPLER_1D:
			sampler = true;
			type = ST_SAMPLER_1D;
			break;
		case GL_SAMPLER_2D:
			sampler = true;
			type = ST_SAMPLER_2D;
			break;
		case GL_SAMPLER_3D:
			sampler = true;
			type = ST_SAMPLER_3D;
			break;
		case GL_SAMPLER_CUBE:
			sampler = true;
			type = ST_SAMPLER_CUBE;
			break;
		case GL_SAMPLER_1D_SHADOW:
			sampler = true;
			type = ST_SAMPLER_1D_SHADOW;
			break;
		case GL_SAMPLER_2D_SHADOW:
			sampler = true;
			type = ST_SAMPLER_2D_SHADOW;
			break;
	}
}

void RendererBasePimpl::InitDomainUniformAndSamplers(ShaderProgram * resource, unsigned int domain, GLuint program)
{


	char uniform_name[1000];
	GLsizei name_length;
	GLint uniform_size;
	GLenum uniform_type;
	GLint uniform_count;

	GLuint uniform_location;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

	std::vector<std::pair<std::string, int> > uniform_names;
	std::vector<std::pair<std::string, int> > sampler_names;
	std::vector<UniformParam> uniform_vector;
	std::vector<GLuint> uniform_locations;
	std::vector<SamplerParam> sampler_vector;
	std::vector<GLuint> sampler_locations;

	unsigned int next_offset = 0;

	for (int i = 0; i < uniform_count; ++i)
	{
		glGetActiveUniform(program, i, 1000, &name_length, &uniform_size, &uniform_type, uniform_name);
		uniform_location = glGetUniformLocation(program, uniform_name);

		unsigned char rows = 1;
		unsigned char columns = 1;
		unsigned char type = 0;
		bool sampler = false;
		GetTypeInfo(uniform_type, type, rows, columns, sampler);
		if (sampler)
		{
			sampler_names.push_back(std::make_pair(uniform_name, sampler_names.size()));
			sampler_vector.push_back(SamplerParam());
			sampler_vector.back().type = type;
			sampler_locations.push_back(uniform_location);
		}
		else
		{
			uniform_names.push_back(std::make_pair(uniform_name, uniform_names.size()));
			uniform_vector.push_back(UniformParam());
			uniform_vector.back().array_length = uniform_size;
			uniform_vector.back().row_major = false;
			uniform_vector.back().type = type;
			uniform_vector.back().rows = rows;
			uniform_vector.back().columns = columns;
			uniform_vector.back().uniform_buffer_index = 0;
			uniform_vector.back().uniform_buffer_offset = next_offset;
			uniform_vector.back().uniform_buffer_array_stride = 0;
			uniform_vector.back().uniform_buffer_matrix_stride = rows > 1 ? rows : 0;
			uniform_locations.push_back(uniform_location);
			switch(type)
			{
				case DT_FLOAT:
					next_offset = next_offset + (sizeof(float) * rows * columns * uniform_size);
					break;
				default:
					std::cout << "Other types not supported yet." << std::endl;
					break;
			}
		}
	}

	std::sort(uniform_names.begin(), uniform_names.end());
	std::sort(sampler_names.begin(), sampler_names.end());

	resource->uniforms.Set(uniform_names.size());
	resource->samplers.Set(sampler_names.size());

	GLint uniformblock_count = 0;
	if (GLEW_ARB_uniform_buffer_object)
	{
		char uniformblock_name[1000];
		GLint name_length;
		GLint block_binding;
		GLint block_data_size;
		GLint block_active_uniforms;
		

		glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &uniformblock_count);

		resource->uniform_blocks.Set(uniform_names.size() > 0 ? uniformblock_count + 1 : uniformblock_count); //Add one because we make "no ubo" as 0 index.
	
		unsigned int total_uniforms_in_blocks = 0;
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

			std::vector<GLuint> uniform_offsets(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_OFFSET, reinterpret_cast<GLint*>(&uniform_offsets[0]));
			std::vector<GLuint> uniform_arraystrides(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_ARRAY_STRIDE, reinterpret_cast<GLint*>(&uniform_arraystrides[0]));
			std::vector<GLuint> uniform_matrixstrides(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_MATRIX_STRIDE, reinterpret_cast<GLint*>(&uniform_matrixstrides[0]));
			std::vector<GLuint> uniform_rowmajor(block_active_uniforms, 0);
			glGetActiveUniformsiv(program, block_active_uniforms, &uniform_indices[0], GL_UNIFORM_IS_ROW_MAJOR, reinterpret_cast<GLint*>(&uniform_rowmajor[0]));

			
			unsigned int block_index = uniform_names.size() > 0 ? (i + 1) : i;//Add one because we make "no ubo" as 0 index.
			resource->uniform_blocks[block_index].name = uniformblock_name;
			resource->uniform_blocks[block_index].byte_count = block_data_size;
			resource->uniform_blocks[block_index].uniform_indices.Set(block_active_uniforms);
			total_uniforms_in_blocks += block_active_uniforms;
			for (unsigned int p = 0; p < uniform_indices.size(); ++p)
			{
				uniform_vector[uniform_indices[p]].uniform_buffer_index = block_index;
				uniform_vector[uniform_indices[p]].uniform_buffer_offset = uniform_offsets[p];
				uniform_vector[uniform_indices[p]].uniform_buffer_array_stride = uniform_arraystrides[p];
				uniform_vector[uniform_indices[p]].uniform_buffer_matrix_stride = uniform_matrixstrides[p];
				uniform_vector[uniform_indices[p]].row_major = uniform_rowmajor[p];
			}
			//std::cout << "	Location: " << glGetUniformLocation(pimpl->combined, uniformblock_name) << std::endl;
		}
		resource->uniform_blocks[0].uniform_indices.Set(uniform_names.size() - total_uniforms_in_blocks);
	}
	else
	{
		if (uniform_names.size() > 0) 
		{
			resource->uniform_blocks.Set(1);
			resource->uniform_blocks[0].uniform_indices.Set(uniform_names.size());
		}
	}

	if (uniform_names.size() > 0)
	{
		std::vector<unsigned int> block_param_count(resource->uniform_blocks.GetSize(), 0);
		unsigned int block_zero_size = 0;
		//Loop through all uniforms sorted by names.
		for (unsigned int i = 0; i < uniform_names.size(); ++i)
		{
			//Set uniform info in ShaderProgram.domains[domain].uniforms.
			int index = uniform_names[i].second;
			resource->uniforms[i] = uniform_vector[index];
			resource->uniforms[i].name = uniform_names[i].first;

			//Put index in the parameter's block in ShaderProgram.domains[domain].uniform_blocks.
			int uniform_block_index = uniform_vector[index].uniform_buffer_index;
			resource->uniform_blocks[uniform_block_index].uniform_indices[block_param_count[uniform_block_index]] = i;
			block_param_count[uniform_block_index] += 1;

			//For block zero, calculate size.
			if (uniform_block_index == 0)
			{
				block_zero_size = std::max<unsigned int>(
					block_zero_size, 
					uniform_vector[index].uniform_buffer_offset + uniform_vector[index].rows * uniform_vector[index].columns * sizeof(float) * uniform_vector[index].array_length); //Only for DT_FLOAT. Fix later.
			}
		}
		resource->uniform_blocks[0].byte_count = block_zero_size;
	}


	for (unsigned int i = 0; i < sampler_names.size(); ++i)
	{
		int index = sampler_names[i].second;
		resource->samplers[i] = sampler_vector[index];
		resource->samplers[i].name = sampler_names[i].first;
	}

	ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	program_pimpl->uniform_locations.Set(uniform_names.size());
	program_pimpl->sampler_locations.Set(sampler_names.size());
	program_pimpl->sampler_units.Set(sampler_names.size());
	program_pimpl->block_bind_points.Set(uniform_names.size() > 0 ? (uniformblock_count + 1) : uniformblock_count);
	program_pimpl->bound_uniform_buffers.Set(uniformblock_count + 1, 0);
}

void RendererBasePimpl::InitDomainUniformBufferDecl(ShaderProgram * resource, unsigned int domain, GLuint program)
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
		GLint referenced_by_geometry_shader;
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

