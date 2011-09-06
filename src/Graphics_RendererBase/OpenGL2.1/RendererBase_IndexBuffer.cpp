#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_IndexBuffer.h"
#include "../DescIndexBuffer.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateResource(const DescIndexBuffer & desc, IndexBuffer * resource)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);

	resource->byte_count = desc.ByteCount;
	resource->offset = 0;
	resource->usage = desc.Usage;
	resource->primitive_type = desc.PrimitiveType;
	resource->PRIVATE = (void *) buffer;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.ByteCount, desc.InitialData, BufferUsageMap[desc.Usage]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RendererBase::Delete(IndexBuffer * resource)
{
	GLuint buffer = (GLuint) resource->PRIVATE;
	glDeleteBuffers(1, &buffer);
}
void RendererBase::BindIndexBuffer(IndexBuffer * resource)
{
	if (resource)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
		pimpl->bound_index_buffer = resource;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		pimpl->bound_index_buffer = 0;
	}
}
void * RendererBase::Map(IndexBuffer * resource, MapAccess access)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, MapAccessMap[access]);
}
void RendererBase::Unmap(IndexBuffer * resource)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint) resource->PRIVATE);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}


