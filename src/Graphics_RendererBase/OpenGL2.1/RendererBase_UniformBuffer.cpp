#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../Interface_UniformBuffer.h"
#include "../DescUniformBuffer.h"

#include "ExLib_API_OpenGL.h"

#include "ShaderProgramPimpl.h"
#include "RendererBasePimpl.h"

class UniformBufferPimpl
{
public:
	unsigned char * cpu_data;
	GLuint buffer;
};

void RendererBase::CreateResource(const DescUniformBuffer & desc, UniformBuffer * resource)
{
	resource->domain = desc.domain;
	resource->index = desc.index;
	resource->byte_count = desc.byte_count;
	

	UniformBufferPimpl * resource_pimpl = new UniformBufferPimpl();
	resource_pimpl->cpu_data = 0;
	resource_pimpl->buffer = 0;
	if (desc.index == 0)
	{
		if (desc.byte_count > 0)
		{
			resource_pimpl->cpu_data = new unsigned char [desc.byte_count];
		}
	}
	else if (GLEW_ARB_uniform_buffer_object)
	{
		glGenBuffers(1, &resource_pimpl->buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, resource_pimpl->buffer);
		glBufferData(GL_UNIFORM_BUFFER, desc.byte_count, 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		resource->index = 0;
	}
	resource->PRIVATE = (void *) resource_pimpl;

	
}
void RendererBase::Delete(UniformBuffer * resource)
{
	UniformBufferPimpl * resource_pimpl = (UniformBufferPimpl *) resource->PRIVATE;
	if (resource->index == 0)
	{
		delete [] resource_pimpl->cpu_data;
	}
	else if (GLEW_ARB_uniform_buffer_object)
	{
		glDeleteBuffers(1, &resource_pimpl->buffer); 
	}
	delete resource_pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::BindUniformBuffer(UniformBuffer * resource, ShaderProgram * program)
{
	BindShaderProgram(program);
	UniformBufferPimpl * resource_pimpl = (UniformBufferPimpl *) resource->PRIVATE;
	if (resource->index == 0)
	{
		ShaderProgramPimpl * shader_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
		//ShaderDomain & shader_domain = program->domains[resource->domain];
		//ShaderDomainPimpl * domain_pimpl = (ShaderDomainPimpl *) shader_domain.PRIVATE;

		if (shader_pimpl->bound_uniform_buffers[0] != resource)
		{
			shader_pimpl->bound_uniform_buffers[0] = resource;
			for (unsigned int i = 0; i < program->uniforms.GetSize(); ++i)
			{
				if (program->GetUniformBufferIndex(i)) continue;

				unsigned char rows = program->GetUniformRows(i);
				unsigned char columns = program->GetUniformColumns(i);
				GLuint location = shader_pimpl->uniform_locations[i];
				unsigned int count = program->GetUniformArrayLength(i);
				float * data = (float *) (resource_pimpl->cpu_data + program->GetUniformBufferOffset(i));

				switch(rows)
				{
					case 1:
						switch (columns)
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
						break;
					case 2:
						switch (columns)
						{
							case 2:
								glUniformMatrix2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix2x3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix2x4fv(location, count, false, data);
								break;
						}
						break;
					case 3:
						switch (columns)
						{
							case 2:
								glUniformMatrix3x2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix3x4fv(location, count, false, data);
								break;
						}
						break;
					case 4:
						switch (columns)
						{
							case 2:
								glUniformMatrix4x2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix4x3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix4fv(location, count, false, data);
								break;
						}
						break;
				}
			}
			//resource_pimpl->dirty_data = false;
		}
	}
	else if (GLEW_ARB_uniform_buffer_object)
	{
		ShaderProgramPimpl * shader_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
		glBindBufferRange(GL_UNIFORM_BUFFER, shader_pimpl->block_bind_points[resource->index], resource_pimpl->buffer, 0, resource->byte_count);
		//glBindBufferBase(GL_UNIFORM_BUFFER, domain_pimpl->block_bind_points[resource->index], resource_pimpl->buffer);
	}
}
unsigned char * RendererBase::Map(UniformBuffer * resource)
{
	UniformBufferPimpl * resource_pimpl = (UniformBufferPimpl *) resource->PRIVATE;
	if (resource->index == 0)
	{
		return resource_pimpl->cpu_data;
	}
	else if (GLEW_ARB_uniform_buffer_object)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, resource_pimpl->buffer);
		unsigned char * rtn = (unsigned char *) glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	}
	else
	{
		return 0;
	}
}
void RendererBase::Unmap(UniformBuffer * resource)
{
	if (resource->index > 0 && GLEW_ARB_uniform_buffer_object)
	{
		UniformBufferPimpl * resource_pimpl = (UniformBufferPimpl *) resource->PRIVATE;
		glBindBuffer(GL_UNIFORM_BUFFER, resource_pimpl->buffer);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	else if (resource->index == 0)
	{
		UniformBufferPimpl * resource_pimpl = (UniformBufferPimpl *) resource->PRIVATE;
		if (pimpl->bound_shader_program) //Update uniforms if a shader program is bound.
		{
			ShaderProgram * program = pimpl->bound_shader_program;
			ShaderProgramPimpl * shader_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
			for (unsigned int i = 0; i < program->uniforms.GetSize(); ++i)
			{
				if (program->GetUniformBufferIndex(i)) continue;

				unsigned char rows = program->GetUniformRows(i);
				unsigned char columns = program->GetUniformColumns(i);
				GLuint location = shader_pimpl->uniform_locations[i];
				unsigned int count = program->GetUniformArrayLength(i);
				float * data = (float *) (resource_pimpl->cpu_data + program->GetUniformBufferOffset(i));

				switch(rows)
				{
					case 1:
						switch (columns)
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
						break;
					case 2:
						switch (columns)
						{
							case 2:
								glUniformMatrix2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix2x3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix2x4fv(location, count, false, data);
								break;
						}
						break;
					case 3:
						switch (columns)
						{
							case 2:
								glUniformMatrix3x2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix3x4fv(location, count, false, data);
								break;
						}
						break;
					case 4:
						switch (columns)
						{
							case 2:
								glUniformMatrix4x2fv(location, count, false, data);
								break;
							case 3:
								glUniformMatrix4x3fv(location, count, false, data);
								break;
							case 4:
								glUniformMatrix4fv(location, count, false, data);
								break;
						}
						break;
				}
			}
		}
	}
}