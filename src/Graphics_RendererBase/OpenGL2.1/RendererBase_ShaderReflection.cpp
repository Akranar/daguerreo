#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../DescShaderProgram.h"

#include <iostream>

#include "ShaderProgramPimpl.h"
#include "ShaderReflectionPimpl.h"

//*************************************
//	ShaderProgram
//*************************************

#include <algorithm>

void RendererBase::CreateShaderReflection(ShaderProgram * resource, ShaderReflection * reflection)
{
	/*
	ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	GLuint program = resource_pimpl->combined;


	std::vector<std::pair<std::string, int> > uniform_name_index_vector;
	std::vector<GLint> uniform_size_vector;
	std::vector<GLenum> uniform_type_vector;

	std::vector<std::pair<std::string, int> > attrib_name_index_vector;
	std::vector<GLint> attrib_size_vector;
	std::vector<GLenum> attrib_type_vector;

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

			uniform_name_index_vector.push_back(std::make_pair(uniform_name, i));
			uniform_size_vector.push_back(uniform_size);
			uniform_type_vector.push_back(uniform_type);
		}
	}

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

			attrib_name_index_vector.push_back(std::make_pair(attrib_name, i));
			attrib_size_vector.push_back(attrib_size);
			attrib_type_vector.push_back(attrib_type);
		}
	}

	std::sort(uniform_name_index_vector.begin(), uniform_name_index_vector.end());
	std::sort(attrib_name_index_vector.begin(), attrib_name_index_vector.end());

	

	ShaderReflectionPimpl * reflection_pimpl = new ShaderReflectionPimpl();
	reflection->PRIVATE = (void *) reflection_pimpl;

	for (unsigned int i = 0; i < uniform_name_index_vector.size(); ++i)
	{
		const unsigned int index = uniform_name_index_vector[i].second;
		GLint uniform_size = uniform_size_vector[index];
		GLenum uniform_type = uniform_type_vector[index];

		reflection_pimpl->uniform_names.push_back(uniform_name_index_vector[i].first);
		reflection_pimpl->uniform_locations.push_back(index);
		reflection_pimpl->uniform_sizes.push_back(uniform_size);
	}

	for (unsigned int i = 0; i < attrib_name_index_vector.size(); ++i)
	{
		const unsigned int index = attrib_name_index_vector[i].second;
		GLint attrib_size = attrib_size_vector[index];
		GLenum attrib_type = attrib_type_vector[index];

		reflection_pimpl->attrib_names.push_back(attrib_name_index_vector[i].first);
		reflection_pimpl->attrib_locations.push_back(index);
		reflection_pimpl->attrib_sizes.push_back(attrib_size);
	}
	int x = 0;
	*/
}
void RendererBase::DeleteShaderReflection(ShaderReflection * reflection)
{
	/*
	ShaderReflectionPimpl * reflection_pimpl = (ShaderReflectionPimpl *) reflection->PRIVATE;
	delete reflection_pimpl;
	reflection->PRIVATE = 0;
	*/
}