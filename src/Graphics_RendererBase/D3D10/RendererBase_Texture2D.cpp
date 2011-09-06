#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_Texture2D.h"
#include "../DescTexture2D.h"

#include "../Interface_ShaderProgram.h"
#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"


#include <iostream>


//*************************************
//	Texture2D
//*************************************

class Texture2DPimpl
{
public:
	ID3D10Texture2D * texture;
	ID3D10ShaderResourceView * shader_resource_view;
	ID3D10RenderTargetView * render_target_view;
	ID3D10DepthStencilView * depthstencil_view;
};

void RendererBase::CreateResource(const DescTexture2D & desc, Texture2D * resource)
{
	resource->width = desc.Width;
	resource->height = desc.Height;
	resource->miplevels = desc.MipLevels;

	ID3D10Texture2D * texture;

	D3D10_TEXTURE2D_DESC t2dd;
	t2dd.Width = desc.Width;
	t2dd.Height = desc.Height;
	t2dd.MipLevels = 1;
	t2dd.ArraySize = 1;
	
	//128-bit
		//t2dd.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	//96-bit
		//t2dd.Format = DXGI_FORMAT_R32G32B32_TYPELESS; //SHADER_RESOURCE ONLY
	//64-bit
		//t2dd.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R32G32_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R32G8X24_TYPELESS;	//SHADER_RESOURCE ONLY
	//32-bit
		t2dd.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R16G16_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R32_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R10G10B10A2_TYPELESS;
		//t2dd.Format = DXGI_FORMAT_R24G8_TYPELESS; //DEPTH_STENCIL ONLY
	
	t2dd.SampleDesc.Count = 1;
	//t2dd.Usage = D3D10_USAGE_DYNAMIC; //SHADER_RESOURCE ONLY
	t2dd.Usage = D3D10_USAGE_DEFAULT;
	t2dd.BindFlags = 0;
	//t2dd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; //SHADER_RESOURCE ONLY
	t2dd.CPUAccessFlags = 0;
	t2dd.MiscFlags = 0;

	if (desc.BindFlags & BIND_SHADER_RESOURCE)
	{
		t2dd.BindFlags |= D3D10_BIND_SHADER_RESOURCE;
	}
	if (desc.BindFlags & BIND_RENDER_TARGET)
	{
		t2dd.BindFlags |= D3D10_BIND_RENDER_TARGET;
	}
	if (desc.BindFlags & BIND_DEPTH_STENCIL)
	{
		t2dd.BindFlags |= D3D10_BIND_DEPTH_STENCIL;
	}
	//Can never have BIND_RENDER_TARGET & BIND_DEPTH_STENCIL
	
	if (desc.InitialData)
	{
		D3D10_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = desc.InitialData;
		subresource_data.SysMemPitch = desc.Width * 4; //bytes per texel;
		subresource_data.SysMemSlicePitch = 0;
		bool failed = FAILED(pimpl->d3d_device->CreateTexture2D(&t2dd, &subresource_data, &texture));
		if (failed)
		{
			std::cout << "NewTexture2D failed." << std::endl;
		}
	}
	else
	{
		bool failed = FAILED(pimpl->d3d_device->CreateTexture2D(&t2dd, NULL, &texture));
		if (failed)
		{
			std::cout << "NewTexture2D failed." << std::endl;
		}
	}
	
	ID3D10ShaderResourceView * shader_view = 0;
	if (desc.BindFlags & BIND_SHADER_RESOURCE)
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//desc.Format;
		view_desc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.MostDetailedMip = 0;
		bool view_failed = FAILED(pimpl->d3d_device->CreateShaderResourceView(texture, &view_desc, &shader_view));
	}
	ID3D10RenderTargetView * target_view = 0;
	if (desc.BindFlags & BIND_RENDER_TARGET)
	{
		D3D10_RENDER_TARGET_VIEW_DESC rt_desc;

		rt_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//desc.Format;
		rt_desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
		rt_desc.Texture2D.MipSlice = 0;
		bool view_failed = FAILED(pimpl->d3d_device->CreateRenderTargetView(texture, &rt_desc, &target_view));
	}
	ID3D10DepthStencilView * depthstencil_view = 0;
	if (desc.BindFlags & BIND_DEPTH_STENCIL)
	{
		D3D10_DEPTH_STENCIL_VIEW_DESC ds_desc;

		ds_desc.Format = DXGI_FORMAT_D32_FLOAT;//desc.Format;
		ds_desc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		ds_desc.Texture2D.MipSlice = 0;
		bool view_failed = FAILED(pimpl->d3d_device->CreateDepthStencilView(texture, &ds_desc, &depthstencil_view));
	}

	Texture2DPimpl * resource_pimpl = new Texture2DPimpl();
	resource_pimpl->texture = texture;
	resource_pimpl->shader_resource_view = shader_view;
	resource_pimpl->render_target_view = target_view;
	resource_pimpl->depthstencil_view = depthstencil_view;

	resource->PRIVATE = (void *) resource_pimpl;
	
}
void RendererBase::Delete(Texture2D * resource)
{
	Texture2DPimpl * resource_pimpl = (Texture2DPimpl *) resource->PRIVATE;
	resource_pimpl->shader_resource_view->Release();
	resource_pimpl->texture->Release();
	delete resource_pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::Bind(ShaderProgram * program, unsigned int unit, Texture2D * resource)
{
	if (resource)
	{
		Texture2DPimpl * resource_pimpl = (Texture2DPimpl *) resource->PRIVATE;
		ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
		switch(program_pimpl->sampler_domains[unit])
		{
			case DOMAIN_VERTEX:
				pimpl->d3d_device->VSSetShaderResources(program_pimpl->sampler_bind_points[unit], 1, &resource_pimpl->shader_resource_view);
				break;
			case DOMAIN_FRAGMENT:
				pimpl->d3d_device->PSSetShaderResources(program_pimpl->sampler_bind_points[unit], 1, &resource_pimpl->shader_resource_view);
				break;
		}
	}
	else
	{
		ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) program;
		switch(program_pimpl->sampler_domains[unit])
		{
			case DOMAIN_VERTEX:
				pimpl->d3d_device->VSSetShaderResources(program_pimpl->sampler_bind_points[unit], 1, 0);
				break;
			case DOMAIN_FRAGMENT:
				pimpl->d3d_device->PSSetShaderResources(program_pimpl->sampler_bind_points[unit], 1, 0);
				break;
		}
	}
}
void * RendererBase::Map(Texture2D * resource, MapAccess access)
{
	Texture2DPimpl * resource_pimpl = (Texture2DPimpl *) resource->PRIVATE;
	return 0;
}
void RendererBase::Unmap(Texture2D * resource)
{
	Texture2DPimpl * resource_pimpl = (Texture2DPimpl *) resource->PRIVATE;
}
