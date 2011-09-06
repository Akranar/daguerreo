#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_TextureCube.h"
#include "../Interface_ShaderProgram.h"
#include "../DescTextureCube.h"

#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"

#include <iostream>

//*************************************
//	TextureCube
//*************************************

class TextureCubePimpl
{
public:
	ID3D10Texture2D * texture;
	ID3D10ShaderResourceView * shader_resource_view;
	ID3D10RenderTargetView * render_target_view;
	ID3D10DepthStencilView * depthstencil_view;
};

void RendererBase::CreateResource(const DescTextureCube & desc, TextureCube * resource)
{
	resource->width = desc.Width;
	resource->height = desc.Height;
	resource->miplevels = desc.MipLevels;

	ID3D10Texture2D * texture;

	D3D10_TEXTURE2D_DESC t2dd;
	t2dd.Width = desc.Width;
	t2dd.Height = desc.Height;
	t2dd.MipLevels = 1;
	t2dd.ArraySize = 6;//Cube map.
	
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
	t2dd.SampleDesc.Quality = 0;
	//t2dd.Usage = D3D10_USAGE_DYNAMIC; //SHADER_RESOURCE ONLY
	t2dd.Usage = D3D10_USAGE_DEFAULT;
	t2dd.BindFlags = 0;
	//t2dd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; //SHADER_RESOURCE ONLY
	t2dd.CPUAccessFlags = 0;
	t2dd.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

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
		D3D10_SUBRESOURCE_DATA subresource_data[6];
		for (unsigned int i = 0; i < 6; ++i)
		{
			subresource_data[i].pSysMem = desc.InitialData[i];
			subresource_data[i].SysMemPitch = desc.Width * 4; //bytes per texel;
			subresource_data[i].SysMemSlicePitch = 0;
		}
		bool failed = FAILED(pimpl->d3d_device->CreateTexture2D(&t2dd, subresource_data, &texture));
		if (failed)
		{
			std::cout << "NewTextureCube failed." << std::endl;
		}
	}
	else
	{
		HRESULT texture_create_result = pimpl->d3d_device->CreateTexture2D(&t2dd, NULL, &texture);
		bool failed = FAILED(texture_create_result);
		if (failed)
		{
			std::cout << "NewTextureCube failed." << std::endl;
		}
	}
	
	ID3D10ShaderResourceView * shader_view = 0;
	if (desc.BindFlags & BIND_SHADER_RESOURCE)
	{
		D3D10_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//desc.Format;
		view_desc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
		view_desc.TextureCube.MipLevels = 1;
		view_desc.TextureCube.MostDetailedMip = 0;
		bool view_failed = FAILED(pimpl->d3d_device->CreateShaderResourceView(texture, &view_desc, &shader_view));
		int x = 0;
	}
	ID3D10RenderTargetView * target_view = 0;
	if (desc.BindFlags & BIND_RENDER_TARGET)
	{
		D3D10_RENDER_TARGET_VIEW_DESC rt_desc;

		rt_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//desc.Format;
		rt_desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;
		rt_desc.Texture2DArray.MipSlice = 0;
		rt_desc.Texture2DArray.FirstArraySlice = 0;
		rt_desc.Texture2DArray.ArraySize = 6;
		bool view_failed = FAILED(pimpl->d3d_device->CreateRenderTargetView(texture, &rt_desc, &target_view));
	}
	ID3D10DepthStencilView * depthstencil_view = 0;
	if (desc.BindFlags & BIND_DEPTH_STENCIL)
	{
		D3D10_DEPTH_STENCIL_VIEW_DESC ds_desc;

		ds_desc.Format = DXGI_FORMAT_D32_FLOAT;//desc.Format;
		ds_desc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DARRAY;
		ds_desc.Texture2DArray.MipSlice = 0;
		ds_desc.Texture2DArray.FirstArraySlice = 0;
		ds_desc.Texture2DArray.ArraySize = 6;
		bool view_failed = FAILED(pimpl->d3d_device->CreateDepthStencilView(texture, &ds_desc, &depthstencil_view));
	}

	TextureCubePimpl * resource_pimpl = new TextureCubePimpl();
	resource_pimpl->texture = texture;
	resource_pimpl->shader_resource_view = shader_view;
	resource_pimpl->render_target_view = target_view;
	resource_pimpl->depthstencil_view = depthstencil_view;

	resource->PRIVATE = (void *) resource_pimpl;
}
void RendererBase::Delete(TextureCube * resource)
{
	TextureCubePimpl * resource_pimpl = (TextureCubePimpl *) resource->PRIVATE;
	resource_pimpl->shader_resource_view->Release();
	resource_pimpl->texture->Release();
	delete resource_pimpl;
	resource->PRIVATE = 0;
}
void RendererBase::Bind(ShaderProgram * program, unsigned int unit, TextureCube * resource)
{
	if (resource)
	{
		TextureCubePimpl * resource_pimpl = (TextureCubePimpl *) resource->PRIVATE;
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
void * RendererBase::Map(TextureCube * resource, MapAccess access, unsigned int face)
{
	TextureCubePimpl * resource_pimpl = (TextureCubePimpl *) resource->PRIVATE;
	return 0;
}
void RendererBase::Unmap(TextureCube * resource, unsigned int face)
{
	TextureCubePimpl * resource_pimpl = (TextureCubePimpl *) resource->PRIVATE;
}