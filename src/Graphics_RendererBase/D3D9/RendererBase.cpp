#include "../RendererBase.h"
#include "RendererBasePimpl.h"


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
	HRESULT result = pimpl->GetDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, pimpl->clear_color, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearColorBuffer()
{
	pimpl->GetDevice()->Clear(0, 0, D3DCLEAR_TARGET, pimpl->clear_color, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearDepthBuffer()
{
	pimpl->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER, pimpl->clear_color, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearStencilBuffer()
{
	pimpl->GetDevice()->Clear(0, 0, D3DCLEAR_STENCIL, pimpl->clear_color, pimpl->clear_depth, pimpl->clear_stencil);
}
void RendererBase::ClearDepthStencilBuffer()
{
	pimpl->GetDevice()->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, pimpl->clear_color, pimpl->clear_depth, pimpl->clear_stencil);
}

void RendererBase::ClearColor(float r, float g, float b, float a)
{
	pimpl->clear_color = D3DCOLOR_RGBA((int)(r*255), (int)(g*255), (int)(b*255), (int)(a*255));
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

void RendererBase::PostRender()
{

}

void RendererBase::Draw()
{
	//UNIMPLEMENTED
}

void RendererBase::OnViewportUpdate(int x, int y, int width, int height)
{

}