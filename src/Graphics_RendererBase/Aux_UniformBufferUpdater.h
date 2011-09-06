#ifndef _UNIFORM_BUFFER_UPDATER_H_
#define _UNIFORM_BUFFER_UPDATER_H_

#include "RendererBase.h"
#include "Interface_UniformBuffer.h"
#include "Interface_ShaderProgram.h"
#include <cstring>


class UniformBufferUpdater
{
	unsigned char * mapped_data;
	RendererBase * renderer;
	ShaderProgram * program;
	UniformBuffer * uniform_buffer;
	unsigned int next_update;
public:
	UniformBufferUpdater(RendererBase * _renderer, ShaderProgram * _program, UniformBuffer * _uniform_buffer)
		: renderer(_renderer), program(_program), uniform_buffer(_uniform_buffer), next_update(0)
	{
		mapped_data = renderer->Map(uniform_buffer);
	}

	UniformBufferUpdater()
		: renderer(0), program(0), uniform_buffer(0), mapped_data(0), next_update(0)
	{

	}

	void Begin(RendererBase * _renderer, ShaderProgram * _program, UniformBuffer * _uniform_buffer)
	{
		renderer = _renderer;
		program = _program;
		uniform_buffer = _uniform_buffer;
		mapped_data = renderer->Map(uniform_buffer);
		next_update = 0;
	}

	void End()
	{
		if (renderer)
		{
			renderer->Unmap(uniform_buffer);
			renderer = 0;
			program = 0;
			uniform_buffer = 0;
			mapped_data = 0;
		}
	}

	~UniformBufferUpdater()
	{
		if (renderer)
		{
			renderer->Unmap(uniform_buffer);
		}
	}

	void Set(unsigned int uniform_index, float * src, unsigned int src_size)
	{
		if (mapped_data)
		{
			memcpy(mapped_data + program->GetUniformBufferOffset(uniform_index), src, src_size);
		}
	}

	void SetInOrder(float * src, unsigned int src_size)
	{
		if (mapped_data)
		{
			unsigned int offset = program->GetUniformBufferOffset(next_update);
			memcpy(mapped_data + offset, src, src_size);
			next_update += 1;
		}
	}
};

#endif