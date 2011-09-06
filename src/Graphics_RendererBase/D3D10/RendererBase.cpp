#include "../RendererBase.h"
#include "RendererBasePimpl.h"

#include "Mapping.h"

#include "../Interface_IndexBuffer.h"

RendererBase::RendererBase()
: pimpl(0)
{
	PlatformIndependentInit();
}

RendererBase::~RendererBase()
{
	DestroyHashers();
	if (pimpl)
	{
		DestroyState(&DefaultBlend);
		DestroyState(&DefaultDepthStencil);
		DestroyState(&DefaultRasterizer);
		DestroyState(&DefaultSampler);
		delete pimpl;
	}
}

void RendererBase::Initialize(WindowEvents * window)
{
	pimpl = new RendererBasePimpl(window);
	CreateDefaultStates(&DefaultBlend, &DefaultDepthStencil, &DefaultRasterizer, &DefaultSampler);
	SetState(&DefaultBlend);
	SetState(&DefaultDepthStencil);
	SetState(&DefaultRasterizer);
}
void RendererBase::ClearAllBuffer()
{
	pimpl->d3d_device->ClearRenderTargetView(pimpl->render_target_view, pimpl->clear_color);
	pimpl->d3d_device->ClearDepthStencilView(pimpl->depth_stencil_view, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearColorBuffer()
{
	pimpl->d3d_device->ClearRenderTargetView(pimpl->render_target_view, pimpl->clear_color);
}
void RendererBase::ClearDepthBuffer()
{
	pimpl->d3d_device->ClearDepthStencilView(pimpl->depth_stencil_view, D3D10_CLEAR_DEPTH, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearStencilBuffer()
{
	pimpl->d3d_device->ClearDepthStencilView(pimpl->depth_stencil_view, D3D10_CLEAR_STENCIL, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearDepthStencilBuffer()
{
	pimpl->d3d_device->ClearDepthStencilView(pimpl->depth_stencil_view, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearColor(float r, float g, float b, float a)
{
	pimpl->clear_color[0] = r;
	pimpl->clear_color[1] = g;
	pimpl->clear_color[2] = b;
	pimpl->clear_color[3] = a;
}
void RendererBase::ClearDepth(double depth)
{
	pimpl->clear_depth = (float) depth;
}
void RendererBase::ClearStencil(int stencil)
{
	pimpl->clear_stencil = stencil;
}


void RendererBase::PreRender()
{

}
void RendererBase::Draw()
{
	pimpl->d3d_device->IASetPrimitiveTopology(MappingPrimitiveType[pimpl->bound_index_buffer->primitive_type]);
	pimpl->d3d_device->DrawIndexed(pimpl->bound_index_buffer->byte_count / sizeof(unsigned int), 0, 0);
}
void RendererBase::PostRender()
{

}

void RendererBase::OnViewportUpdate(int x, int y, int width, int height)
{
	
}