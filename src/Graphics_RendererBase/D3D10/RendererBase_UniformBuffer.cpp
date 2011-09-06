#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../Interface_UniformBuffer.h"
#include "../DescUniformBuffer.h"


#include "ShaderProgramPimpl.h"
#include "RendererBasePimpl.h"

#include <Core_Exception.h>


void RendererBase::CreateResource(const DescUniformBuffer & desc, UniformBuffer * resource)
{
	resource->byte_count = desc.byte_count;
	resource->index = desc.index;
	resource->domain = desc.domain;


	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = desc.byte_count;
	bd.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	bool failed = FAILED( pimpl->d3d_device->CreateBuffer(&bd, NULL, (ID3D10Buffer **) &resource->PRIVATE));
	CONDITIONAL_EXCEPTION(failed, "UniformBuffer Creation Failed.");
}
void RendererBase::Delete(UniformBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Release();
	resource->PRIVATE = 0;
}
void RendererBase::BindUniformBuffer(UniformBuffer * resource, ShaderProgram * program)
{
	if (resource)
	{
		ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
		unsigned int domain = program_pimpl->block_domains[resource->index];
		unsigned int bind_point = program_pimpl->block_bind_points[resource->index];
		switch (domain)
		{
		case DOMAIN_VERTEX:
			pimpl->d3d_device->VSSetConstantBuffers(bind_point, 1, (ID3D10Buffer **) &resource->PRIVATE);
			break;
		case DOMAIN_FRAGMENT:
			pimpl->d3d_device->PSSetConstantBuffers(bind_point, 1, (ID3D10Buffer **) &resource->PRIVATE);
			break;
		case DOMAIN_GEOMETRY:
			pimpl->d3d_device->GSSetConstantBuffers(bind_point, 1, (ID3D10Buffer **) &resource->PRIVATE);
			break;
		}
	}
}
unsigned char * RendererBase::Map(UniformBuffer * resource)
{
	void * data;
	HRESULT result = ((ID3D10Buffer *) resource->PRIVATE)->Map(D3D10_MAP_WRITE_DISCARD, 0, &data);
	return (unsigned char *) data;
}
void RendererBase::Unmap(UniformBuffer * resource)
{
	((ID3D10Buffer *) resource->PRIVATE)->Unmap();
}