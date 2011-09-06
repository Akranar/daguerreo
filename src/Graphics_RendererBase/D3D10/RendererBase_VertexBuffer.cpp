#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_VertexBuffer.h"
#include "../DescVertexBuffer.h"

#include "RendererBasePimpl.h"

#include <Core_Exception.h>

#include <iostream>

//*************************************
//	VertexBuffer
//*************************************

void RendererBase::CreateResource(const DescVertexBuffer & desc, VertexBuffer * resource)
{
	resource->byte_count = desc.ByteCount;
	resource->byte_stride = desc.ByteStride;
	resource->offset = 0;
	resource->usage = desc.Usage;


	D3D10_BUFFER_DESC bd;
	bd.Usage = MappingBufferUsage[desc.Usage];
	bd.ByteWidth = desc.ByteCount;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
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
		CONDITIONAL_EXCEPTION(failed, "VertexBuffer Creation Failed.");
	}
}

void RendererBase::Delete(VertexBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Release();
	resource->PRIVATE = 0;
}

void RendererBase::BindVertexBuffer(VertexBuffer * resource)
{
	if (resource)
	{
		UINT offset = 0;
		pimpl->d3d_device->IASetVertexBuffers(0, 1, (ID3D10Buffer **) &resource->PRIVATE, &resource->byte_stride, &offset);
	}
}

void * RendererBase::Map(VertexBuffer * resource, MapAccess access)
{
	void * data;
	D3D10_MAP mapped_access = MappingMapAccess[access];
	HRESULT result = ((ID3D10Buffer *) resource->PRIVATE)->Map(mapped_access, 0, &data);
	return data;
}

void RendererBase::Unmap(VertexBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Unmap();
}
