#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_VertexBuffer.h"
#include "../DescVertexBuffer.h"

#include "RendererBasePimpl.h"
#include <d3d9.h>
#include <cstring>

//*************************************
//	VertexBuffer
//*************************************

void RendererBase::CreateResource(const DescVertexBuffer & desc, VertexBuffer * resource)
{
	resource->byte_count = desc.ByteCount;
	resource->byte_stride = desc.ByteStride;
	resource->offset = 0;
	resource->usage = desc.Usage;

	IDirect3DVertexBuffer9 * vertex_buffer;
	HRESULT result = pimpl->d3d_device->CreateVertexBuffer(desc.ByteCount, 0, MappingBufferUsage[desc.Usage], D3DPOOL_DEFAULT, &vertex_buffer, 0);
	resource->PRIVATE = (void *) vertex_buffer;

	//Init
	if (desc.InitialData)
	{	
		void * data = 0;
		vertex_buffer->Lock(0, resource->byte_count, &data, D3DLOCK_DISCARD);
		memcpy(data, desc.InitialData, desc.ByteCount);
		vertex_buffer->Unlock();
	}
}

void RendererBase::Delete(VertexBuffer * resource)
{
	((IDirect3DVertexBuffer9 *) resource->PRIVATE)->Release();
	resource->PRIVATE = 0;
}

void RendererBase::BindVertexBuffer(VertexBuffer * resource)
{
	if (resource)
	{
		IDirect3DVertexBuffer9 * vertex_buffer = (IDirect3DVertexBuffer9 *) resource->PRIVATE;
		HRESULT result = pimpl->d3d_device->SetStreamSource(0, vertex_buffer, resource->offset, resource->byte_stride);
		int x = 0;
	}
	else
	{
		pimpl->d3d_device->SetStreamSource(0, 0, 0, 0);
	}
}

void * RendererBase::Map(VertexBuffer * resource, MapAccess access)
{
	IDirect3DVertexBuffer9 * vertex_buffer = (IDirect3DVertexBuffer9 *) resource->PRIVATE;
	void * data = 0;
	vertex_buffer->Lock(0, resource->byte_count, &data, MappingMapAccess[access]);
	return data;
}

void RendererBase::Unmap(VertexBuffer * resource)
{
	IDirect3DVertexBuffer9 * vertex_buffer = (IDirect3DVertexBuffer9 *) resource->PRIVATE;
	vertex_buffer->Unlock();
}
