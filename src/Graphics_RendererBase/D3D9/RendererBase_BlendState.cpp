#include "../RendererBase.h"
#include "../DescBlendState.h"
#include "../Interface_BlendState.h"

#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescBlendState & desc, BlendState * state)
{
	DescBlendState * state_pimpl = new DescBlendState();
	state_pimpl->SetDefaults();
	state->PRIVATE = (void *) state_pimpl;
}

/*
	bool BlendEnable[8];
	bool RenderTargetWriteMask[8][4];
	bool AlphaToCoverageEnable;
	unsigned char SrcBlend;
	unsigned char DestBlend;
	unsigned char BlendOp;
	unsigned char SrcBlendAlpha;
	unsigned char DestBlendAlpha;
	unsigned char BlendOpAlpha;
*/
	
void RendererBase::SetState(const BlendState * in_blend_state)
{
	const BlendState * blend_state = in_blend_state ? in_blend_state : &DefaultBlend;
	DescBlendState * state_pimpl = (DescBlendState *) blend_state->PRIVATE;

	pimpl->d3d_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);  //special;
	pimpl->d3d_device->SetRenderState(D3DRS_BLENDFACTOR, (DWORD)0x00000001);  //special;
	pimpl->d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pimpl->d3d_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	pimpl->d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, state_pimpl->BlendEnable[0] ? TRUE : FALSE);
	pimpl->d3d_device->SetRenderState(D3DRS_SRCBLEND, MappingBlendFactor[state_pimpl->SrcBlend]);
	pimpl->d3d_device->SetRenderState(D3DRS_DESTBLEND, MappingBlendFactor[state_pimpl->DestBlend]);
	pimpl->d3d_device->SetRenderState(D3DRS_BLENDOP, MappingBlendOp[state_pimpl->BlendOp]);
	pimpl->d3d_device->SetRenderState(D3DRS_SRCBLENDALPHA, MappingBlendFactor[state_pimpl->SrcBlendAlpha]);
	pimpl->d3d_device->SetRenderState(D3DRS_DESTBLENDALPHA, MappingBlendFactor[state_pimpl->DestBlendAlpha]);
	pimpl->d3d_device->SetRenderState(D3DRS_BLENDOPALPHA, MappingBlendOp[state_pimpl->BlendOpAlpha]);
	UINT render_target_write_mask = 0;
	if (state_pimpl->RenderTargetWriteMask[0][0])
	{
		render_target_write_mask |= D3DCOLORWRITEENABLE_RED;
	}
	if (state_pimpl->RenderTargetWriteMask[0][1])
	{
		render_target_write_mask |= D3DCOLORWRITEENABLE_GREEN;
	}
	if (state_pimpl->RenderTargetWriteMask[0][2])
	{
		render_target_write_mask |= D3DCOLORWRITEENABLE_BLUE;
	}
	if (state_pimpl->RenderTargetWriteMask[0][3])
	{
		render_target_write_mask |= D3DCOLORWRITEENABLE_ALPHA;
	}
	pimpl->d3d_device->SetRenderState(D3DRS_COLORWRITEENABLE, render_target_write_mask);
	
	//D3DRS_BLENDFACTOR
	//D3DRS_SEPARATEALPHABLENDENABLE
	//D3DRS_SRCBLENDALPHA and D3DRS_DESTBLENDALPHA
	//pimpl->d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
}
	
void RendererBase::DestroyState(BlendState * state)
{
	DescBlendState * state_pimpl = (DescBlendState *) state->PRIVATE;
	delete state_pimpl;
	state->PRIVATE = 0;
}
	


