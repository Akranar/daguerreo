#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_IndexBuffer.h"
#include "../DescIndexBuffer.h"

#include "RendererBasePimpl.h"

void RendererBase::CreateResource(const DescIndexBuffer & desc, IndexBuffer * resource)
{
	resource->byte_count = desc.ByteCount;
	resource->offset = 0;
	resource->usage = desc.Usage;
	resource->primitive_type = desc.PrimitiveType;

	IDirect3DIndexBuffer9 * index_buffer;
	pimpl->d3d_device->CreateIndexBuffer(desc.ByteCount, MappingBufferUsage[desc.Usage], D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, 0);
	resource->PRIVATE = (void *) index_buffer;

	//Init
	if (desc.InitialData)
	{	
		void * data = 0;
		index_buffer->Lock(0, resource->byte_count, &data, D3DLOCK_DISCARD);
		memcpy(data, desc.InitialData, desc.ByteCount);
		index_buffer->Unlock();
	}
}

void RendererBase::Delete(IndexBuffer * resource)
{
	IDirect3DIndexBuffer9 * index_buffer = (IDirect3DIndexBuffer9 *) resource->PRIVATE;
	index_buffer->Release();
}
void RendererBase::BindIndexBuffer(IndexBuffer * resource)
{
	if (resource)
	{
		IDirect3DIndexBuffer9 * index_buffer = (IDirect3DIndexBuffer9 *) resource->PRIVATE;
		HRESULT result = pimpl->d3d_device->SetIndices(index_buffer);
		int x = 0;
	}
	else
	{
		pimpl->d3d_device->SetIndices(0);
	}
}
void * RendererBase::Map(IndexBuffer * resource, MapAccess access)
{
	IDirect3DIndexBuffer9 * index_buffer = (IDirect3DIndexBuffer9 *) resource->PRIVATE;
	void * data = 0;
	index_buffer->Lock(0, resource->byte_count, &data, MappingMapAccess[access]);
	return data;
}
void RendererBase::Unmap(IndexBuffer * resource)
{
	IDirect3DIndexBuffer9 * index_buffer = (IDirect3DIndexBuffer9 *) resource->PRIVATE;
	index_buffer->Unlock();
}

