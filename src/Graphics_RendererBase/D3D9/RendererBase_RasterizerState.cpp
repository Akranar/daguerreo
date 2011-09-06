#include "../RendererBase.h"
#include "../DescRasterizerState.h"
#include "../Interface_RasterizerState.h"


#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescRasterizerState & desc, RasterizerState * state)
{
	DescRasterizerState * state_pimpl = new DescRasterizerState();
	state_pimpl->SetDefaults();
	state->PRIVATE = (void *) state_pimpl;
}

/*
	int ScissorRect[4];
	float DepthBias;
	float SlopeScaledDepthBias;
	unsigned char PolygonMode;
	unsigned char CullFace;
	bool FrontCounterClockwise;
	bool DepthClipEnable;
	bool ScissorEnable;
*/
	
void RendererBase::SetState(const RasterizerState * in_state)
{
	const RasterizerState * state = in_state ? in_state : &DefaultRasterizer;
	DescRasterizerState * state_pimpl = (DescRasterizerState *) state->PRIVATE;

	if (state_pimpl->FrontCounterClockwise)
	{
		pimpl->d3d_device->SetRenderState(D3DRS_CULLMODE, MappingRSCull_FrontCCW[state_pimpl->CullFace]);
	}
	else
	{
		pimpl->d3d_device->SetRenderState(D3DRS_CULLMODE, MappingRSCull_FrontCW[state_pimpl->CullFace]);
	}
	pimpl->d3d_device->SetRenderState(D3DRS_FILLMODE, MappingRSFill[state_pimpl->PolygonMode]);
	if (state_pimpl->DepthClipEnable)
	{
		pimpl->d3d_device->SetRenderState(D3DRS_DEPTHBIAS, state_pimpl->DepthBias);
		pimpl->d3d_device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, state_pimpl->SlopeScaledDepthBias);
	}
	else
	{
		pimpl->d3d_device->SetRenderState(D3DRS_DEPTHBIAS, 0);
		pimpl->d3d_device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	}
	pimpl->d3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, state_pimpl->ScissorEnable ? TRUE : FALSE);
}
	

void RendererBase::DestroyState(RasterizerState * state)
{
	DescRasterizerState * state_pimpl = (DescRasterizerState *) state->PRIVATE;
	delete state_pimpl;
	state->PRIVATE = 0;
}
	
