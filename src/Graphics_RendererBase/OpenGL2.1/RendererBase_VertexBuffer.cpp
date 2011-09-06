#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_VertexBuffer.h"
#include "../DescVertexBuffer.h"

#include "RendererBasePimpl.h"

//*************************************
//	VertexBuffer
//*************************************

void RendererBase::CreateResource(const DescVertexBuffer & desc, VertexBuffer * resource)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);

	resource->byte_count = desc.ByteCount;
	resource->byte_stride = desc.ByteStride;
	resource->offset = 0;
	resource->usage = desc.Usage;
	resource->PRIVATE = (void *) buffer;

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, desc.ByteCount, desc.InitialData, BufferUsageMap[desc.Usage]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RendererBase::Delete(VertexBuffer * resource)
{
	GLuint buffer = (GLuint) resource->PRIVATE;
	glDeleteBuffers(1, &buffer);
}

void RendererBase::BindVertexBuffer(VertexBuffer * resource)
{
	if (resource)
	{
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
		/*
		if (pimpl->bound_layout)
		{
			pimpl->bound_layout->Begin();
		}
		*/
		pimpl->bound_vertex_buffer = resource;
	}
	else
	{
		/*
		if (pimpl->bound_layout)
		{
			pimpl->bound_layout->End();
		}
		*/
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		pimpl->bound_vertex_buffer = 0;
	}
}

void * RendererBase::Map(VertexBuffer * resource, MapAccess access)
{
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
	return glMapBuffer(GL_ARRAY_BUFFER, MapAccessMap[access]);
}

void RendererBase::Unmap(VertexBuffer * resource)
{
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}
