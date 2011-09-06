#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_IndexBuffer.h"
#include "../DescIndexBuffer.h"

#include "RendererBasePimpl.h"

//*************************************
//	IndexBuffer
//*************************************

void RendererBase::CreateResource(const DescIndexBuffer & desc, IndexBuffer * resource)
{
	resource->byte_count = desc.ByteCount;
	resource->offset = 0;
	resource->usage = desc.Usage;
	resource->primitive_type = desc.PrimitiveType;

	D3D10_BUFFER_DESC bd;
	bd.Usage = MappingBufferUsage[desc.Usage];
	bd.ByteWidth = desc.ByteCount;
	bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = MappingCPUAccess[desc.Usage];
	bd.MiscFlags = 0;

	if (desc.InitialData)
	{
		D3D10_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = desc.InitialData;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		bool failed = FAILED( pimpl->d3d_device->CreateBuffer(&bd, &subresource_data, (ID3D10Buffer **) &resource->PRIVATE));
	}
	else
	{
		bool failed = FAILED( pimpl->d3d_device->CreateBuffer(&bd, NULL, (ID3D10Buffer **) &resource->PRIVATE));
	}
}

void RendererBase::Delete(IndexBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Release();
	resource->PRIVATE = 0;
}

void RendererBase::BindIndexBuffer(IndexBuffer * resource)
{
	if (resource)
	{
		UINT offset = 0;
		DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
		pimpl->d3d_device->IASetIndexBuffer((ID3D10Buffer *) resource->PRIVATE, format, offset);
		pimpl->bound_index_buffer = resource;
	}
	else
	{
		pimpl->bound_index_buffer = 0;
	}
}

void * RendererBase::Map(IndexBuffer * resource, MapAccess access)
{
	void * data;
	((ID3D10Buffer *) resource->PRIVATE)->Map(MappingMapAccess[access], 0, &data);
	return data;
}

void RendererBase::Unmap(IndexBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Unmap();
}
