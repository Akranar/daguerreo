#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_Texture2D.h"
#include "../DescTexture2D.h"

#include "../Interface_ShaderProgram.h"
#include "ShaderProgramPimpl.h"
#include "RendererBasePimpl.h"
#include <cstring>

//*************************************
//	Texture2D
//*************************************

void RendererBase::CreateResource(const DescTexture2D & desc, Texture2D * resource)
{
	resource->width = desc.Width;
	resource->height = desc.Height;
	resource->miplevels = desc.MipLevels;

	IDirect3DTexture9 * texture;

	DWORD usage = D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC;//
	D3DFORMAT format = D3DFMT_A8R8G8B8;
	HRESULT result = pimpl->d3d_device->CreateTexture(desc.Width, desc.Height, desc.MipLevels, usage, format, D3DPOOL_DEFAULT, &texture, 0);
	
	if (desc.InitialData)
	{
		IDirect3DSurface9 * surface;
		texture->GetSurfaceLevel(0, &surface);

		D3DLOCKED_RECT rect;
		surface->LockRect(&rect, 0, D3DLOCK_DISCARD);
		memcpy(rect.pBits, desc.InitialData, desc.Width * desc.Height * 4);
		surface->UnlockRect();
	}
	
	resource->PRIVATE = (void *) texture;
}
void RendererBase::Delete(Texture2D * resource)
{
	IDirect3DTexture9 * texture = (IDirect3DTexture9 *) resource->PRIVATE;
	texture->Release();
}
void RendererBase::Bind(ShaderProgram * shader_program, unsigned int unit, Texture2D * resource)
{
	if (resource)
	{
		//ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) shader_program->PRIVATE;
		IDirect3DTexture9 * texture = (IDirect3DTexture9 *) resource->PRIVATE;
		//unsigned int bind_point = program_pimpl->sampler_bind_points[unit];
		unsigned int bind_point = unit; //use this for now.  program_pimpl->sampler_bind_points not yet initialized.
		HRESULT result = pimpl->d3d_device->SetTexture(bind_point, texture);
	}
	else
	{
		//ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) shader_program->PRIVATE;
		//unsigned int bind_point = program_pimpl->sampler_bind_points[unit];
		unsigned int bind_point = unit;
		HRESULT result = pimpl->d3d_device->SetTexture(bind_point, 0);
	}
}
void * RendererBase::Map(Texture2D * resource, MapAccess access)
{
	IDirect3DTexture9 * texture = (IDirect3DTexture9 *) resource->PRIVATE;
	D3DLOCKED_RECT rect;
	texture->LockRect(0, &rect, 0, MappingMapAccess[access]);
	return rect.pBits;
}
void RendererBase::Unmap(Texture2D * resource)
{
	IDirect3DTexture9 * texture = (IDirect3DTexture9 *) resource->PRIVATE;
	texture->UnlockRect(0);
}
