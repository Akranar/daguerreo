#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../DescShaderProgram.h"

#include <iostream>

//*************************************
//	ShaderProgram
//*************************************

#include "RendererBasePimpl.h"
#include "ShaderReflectionPimpl.h"
#include "ShaderProgramPimpl.h"
#include <algorithm>

#include "../Interface_UniformUpdater.h"


void RendererBase::CreateResource(const DescShaderProgram & desc, ShaderProgram * resource)
{
	char error_log[1000];
	GLsizei error_log_length;

	ShaderProgramPimpl * shader_pimpl = new ShaderProgramPimpl();
	resource->PRIVATE = (void *) shader_pimpl;

	const char * vertex_shader_text = desc.GetProgramString(0);
	const GLint vertex_shader_length = desc.GetProgramStringLength(0);
	const char * fragment_shader_text = desc.GetProgramString(1);
	const GLint fragment_shader_length = desc.GetProgramStringLength(1);

	//Create OpenGL GLSL Vertex Shader.
	shader_pimpl->vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_pimpl->vertex, 1, &vertex_shader_text, &vertex_shader_length);
	glCompileShader(shader_pimpl->vertex);

	//Create OpenGL GLSL Fragment Shader.
	shader_pimpl->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_pimpl->fragment, 1, &fragment_shader_text, &fragment_shader_length);
	glCompileShader(shader_pimpl->fragment);

	//Create Combined Program.
	shader_pimpl->combined = glCreateProgram();

	//Attach shaders and link.
	glAttachShader(shader_pimpl->combined, shader_pimpl->vertex);
	glAttachShader(shader_pimpl->combined, shader_pimpl->fragment);
	glLinkProgram(shader_pimpl->combined);

	pimpl->InitDomainUniformAndSamplers(resource, 0, shader_pimpl->combined);

	GLint vs_compiled, fs_compiled, program_linked;
	glGetShaderiv(shader_pimpl->vertex, GL_COMPILE_STATUS, &vs_compiled);
	glGetShaderiv(shader_pimpl->fragment, GL_COMPILE_STATUS, &fs_compiled);
	glGetProgramiv(shader_pimpl->combined, GL_LINK_STATUS, &program_linked);

	if (!vs_compiled || !fs_compiled || !program_linked)
	{
		std::string errors;
		glGetShaderInfoLog(shader_pimpl->vertex, 1000, &error_log_length, error_log);
		if (error_log_length > 0) errors.append(error_log);
		glGetShaderInfoLog(shader_pimpl->fragment, 1000, &error_log_length, error_log);
		if (error_log_length > 0) errors.append(error_log);
		glGetProgramInfoLog(shader_pimpl->combined, 1000, &error_log_length, error_log);
		if (error_log_length > 0) errors.append(error_log);
		throw Exception(errors.c_str());
	}

	glUseProgram(shader_pimpl->combined);
	
	//Get uniform / sampler / block locations in each domain.
	GLint next_sampler_unit = 0;
	GLint next_block_location = 0;
	for (unsigned int i = 0; i < resource->uniforms.GetSize(); ++i)
	{
		((ShaderProgramPimpl *) resource->PRIVATE)->uniform_locations[i] =
			glGetUniformLocation(shader_pimpl->combined, resource->uniforms[i].name.c_str());
	}

	for (unsigned int i = 0; i < resource->samplers.GetSize(); ++i)
	{
		GLint sampler_location = glGetUniformLocation(shader_pimpl->combined, resource->samplers[i].name.c_str());
		((ShaderProgramPimpl *) resource->PRIVATE)->sampler_locations[i] = sampler_location;
		//Set unit;
		glUniform1i(sampler_location, next_sampler_unit);
		((ShaderProgramPimpl *) resource->PRIVATE)->sampler_units[i] = next_sampler_unit;
		next_sampler_unit += 1;
	}

	for (unsigned int i = 1; i < resource->uniform_blocks.GetSize(); ++i)
	{
		GLuint block_index = glGetUniformBlockIndex(shader_pimpl->combined, resource->uniform_blocks[i].name.c_str());
		//Set bind point
		glUniformBlockBinding(shader_pimpl->combined, block_index, next_block_location);
		((ShaderProgramPimpl *) resource->PRIVATE)->block_bind_points[i] = next_block_location;
		next_block_location += 1;
	}
	if (resource->uniform_blocks.GetSize() > 0)
	{
		((ShaderProgramPimpl *) resource->PRIVATE)->block_bind_points[0] = -1;
	}



	glUseProgram(0);

	//**********************************
	//	Create DescShaderSignature;
	//**********************************
	{
		char attrib_name[1000];
		GLsizei name_length;
		GLint attrib_size;
		GLenum attrib_type;
		GLint attrib_count;
		glGetProgramiv(shader_pimpl->combined, GL_ACTIVE_ATTRIBUTES, &attrib_count);

		DescShaderSignature sig;
		sig.signature_entries.Set(attrib_count);

		unsigned int channels;

		for (int i = 0; i < attrib_count; ++i)
		{
			glGetActiveAttrib(shader_pimpl->combined, i, 1000, &name_length, &attrib_size, &attrib_type, attrib_name);
			GLuint location = glGetAttribLocation(shader_pimpl->combined, attrib_name);
			switch (attrib_type)
			{
				case GL_FLOAT:
					channels = 1;
					break;
				case GL_FLOAT_VEC2:
					channels = 2;
					break;
				case GL_FLOAT_VEC3:
					channels = 3;
					break;
				case GL_FLOAT_VEC4: 
					channels = 4;
					break;
				case GL_FLOAT_MAT2: 
					channels = 2;
					break;
				case GL_FLOAT_MAT3: 
					channels = 3;
					break;
				case GL_FLOAT_MAT4: 
					channels = 4;
					break;
				case GL_FLOAT_MAT2x3: 
					channels = 2;
					break;
				case GL_FLOAT_MAT2x4: 
					channels = 2;
					break;
				case GL_FLOAT_MAT3x2: 
					channels = 3;
					break;
				case GL_FLOAT_MAT3x4: 
					channels = 3;
					break;
				case GL_FLOAT_MAT4x2: 
					channels = 4;
					break;
				case GL_FLOAT_MAT4x3:
					channels = 4;
					break;
			}

			sig.signature_entries[location].name = attrib_name;


			unsigned int semantic_length = 0;
			unsigned int semantic_index = 0;
			const std::string & semantic_name = sig.signature_entries[location].name;
			pimpl->InputLayoutHelper_NameToUsage(semantic_name, semantic_length, semantic_index);
			if (semantic_length < semantic_name.length())
			{
				sig.signature_entries[location].name.assign(semantic_name.substr(0, semantic_length));
			}

			sig.signature_entries[location].channels = channels;
			sig.signature_entries[location].type = DT_FLOAT;
			sig.signature_entries[location].usage_index = semantic_index;

		}

		DescShaderSignature & inserted_signature_desc = GetShaderSignatureHash(sig, resource->signature_hash);
		resource->signature_desc = &inserted_signature_desc;
	}



}

void RendererBase::Delete(ShaderProgram * resource)
{
	ShaderProgramPimpl * pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	glDetachShader(pimpl->combined, pimpl->fragment);
	glDetachShader(pimpl->combined, pimpl->vertex);
	glDeleteShader(pimpl->vertex);
	glDeleteShader(pimpl->fragment);
	glDeleteProgram(pimpl->combined);

	delete pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::BindShaderProgram(ShaderProgram * resource)
{
	if (pimpl->bound_shader_program == resource) return;
	if (resource)
	{
		glUseProgram(((ShaderProgramPimpl *) resource->PRIVATE)->combined);
		pimpl->bound_shader_program = resource;
		ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) pimpl->bound_shader_program->PRIVATE;
		program_pimpl->bound_uniform_buffers[0] = 0;//Set main (emulated) uniform buffer to 0.
	}
	else
	{
		pimpl->bound_shader_program = 0;
		glUseProgram(0);
	}
}

void RendererBase::SetUniformf(ShaderProgram * resource, unsigned int domain, unsigned int index, unsigned int count, bool transpose, float * data)
{
	UniformParam & param = resource->uniforms[index];
	ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	GLint location = program_pimpl->uniform_locations[index];
	switch(param.columns)
	{
	case 1:
		switch (param.rows)
		{
			case 1:
				glUniform1fv(location, count, data);
				break;
			case 2:
				glUniform2fv(location, count, data);
				break;
			case 3:
				glUniform3fv(location, count, data);
				break;
			case 4:
				glUniform4fv(location, count, data);
				break;
		}
	case 2:
		switch (param.rows)
		{
			case 2:
				glUniformMatrix2fv(location, count, transpose, data);
				break;
			case 3:
				glUniformMatrix2x3fv(location, count, transpose, data);
				break;
			case 4:
				glUniformMatrix2x4fv(location, count, transpose, data);
				break;
		}
	case 3:
		switch (param.rows)
		{
			case 2:
				glUniformMatrix3x2fv(location, count, transpose, data);
				break;
			case 3:
				glUniformMatrix3fv(location, count, transpose, data);
				break;
			case 4:
				glUniformMatrix3x4fv(location, count, transpose, data);
				break;
		}
	case 4:
		switch (param.rows)
		{
			case 2:
				glUniformMatrix4x2fv(location, count, transpose, data);
				break;
			case 3:
				glUniformMatrix4x3fv(location, count, transpose, data);
				break;
			case 4:
				glUniformMatrix4fv(location, count, transpose, data);
				break;
		}
	}
}

