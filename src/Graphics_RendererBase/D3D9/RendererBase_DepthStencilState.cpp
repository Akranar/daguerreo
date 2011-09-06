#include "../RendererBase.h"
#include "../DescDepthStencilState.h"
#include "../Interface_DepthStencilState.h"


#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescDepthStencilState & desc, DepthStencilState * state)
{
	DescDepthStencilState * state_pimpl = new DescDepthStencilState();
	state_pimpl->SetDefaults();
	state->PRIVATE = (void *) state_pimpl;
}

/*
	class FaceStencilDesc
	{
	public:
		unsigned char StencilFailOp;
		unsigned char StencilDepthFailOp;
		unsigned char StencilPassOp;
		unsigned char StencilFunc;
	};

	unsigned int StencilReadMask;
	unsigned int StencilWriteMask;
	unsigned int StencilReference;
	unsigned char DepthFunc;
	FaceStencilDesc FrontFace;
	FaceStencilDesc BackFace;
	bool DepthEnable;
	bool DepthWriteMask;
	bool StencilEnable;
*/
	
void RendererBase::SetState(const DepthStencilState * in_state)
{
	const DepthStencilState * state = in_state ? in_state : &DefaultDepthStencil;
	DescDepthStencilState * state_pimpl = (DescDepthStencilState *) state->PRIVATE;

	pimpl->d3d_device->SetRenderState(D3DRS_STENCILREF, 0); //Special
	pimpl->d3d_device->SetRenderState(D3DRS_ZENABLE, state_pimpl->DepthEnable ? D3DZB_TRUE : D3DZB_FALSE);
	pimpl->d3d_device->SetRenderState(D3DRS_ZWRITEENABLE, state_pimpl->DepthWriteMask ? TRUE : FALSE);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILENABLE, state_pimpl->StencilEnable ? TRUE : FALSE);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILMASK, state_pimpl->StencilReadMask);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILWRITEMASK, state_pimpl->StencilWriteMask);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILFUNC, MappingComparisonMode[state_pimpl->FrontFace.StencilFunc]);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILFAIL, MappingDSOperation[state_pimpl->FrontFace.StencilFailOp]);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILZFAIL, MappingDSOperation[state_pimpl->FrontFace.StencilDepthFailOp]);
	pimpl->d3d_device->SetRenderState(D3DRS_STENCILPASS, MappingDSOperation[state_pimpl->FrontFace.StencilPassOp]);
}

void RendererBase::DestroyState(DepthStencilState * state)
{
	DescDepthStencilState * state_pimpl = (DescDepthStencilState *) state->PRIVATE;
	delete state_pimpl;
	state->PRIVATE = 0;
}
	

