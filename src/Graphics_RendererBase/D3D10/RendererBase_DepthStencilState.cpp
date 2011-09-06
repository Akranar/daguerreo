#include "../RendererBase.h"
#include "../DescDepthStencilState.h"
#include "../Interface_DepthStencilState.h"


#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescDepthStencilState & desc, DepthStencilState * state)
{
	D3D10_DEPTH_STENCIL_DESC ds_desc;
	ID3D10DepthStencilState * ds_state;

	ds_desc.DepthEnable = desc.DepthEnable;
	ds_desc.DepthWriteMask = desc.DepthWriteMask ? D3D10_DEPTH_WRITE_MASK_ALL : D3D10_DEPTH_WRITE_MASK_ZERO;
	ds_desc.DepthFunc = MappingComparisonMode[desc.DepthFunc];
	ds_desc.StencilEnable = desc.StencilEnable;
	ds_desc.StencilReadMask = desc.StencilReadMask;
	ds_desc.StencilWriteMask = desc.StencilWriteMask;
	ds_desc.FrontFace.StencilFailOp = MappingDSOperation[desc.FrontFace.StencilFailOp];
	ds_desc.FrontFace.StencilDepthFailOp = MappingDSOperation[desc.FrontFace.StencilDepthFailOp];
	ds_desc.FrontFace.StencilPassOp = MappingDSOperation[desc.FrontFace.StencilPassOp];
	ds_desc.FrontFace.StencilFunc = MappingComparisonMode[desc.FrontFace.StencilFunc];
	ds_desc.BackFace.StencilFailOp = MappingDSOperation[desc.BackFace.StencilFailOp];
	ds_desc.BackFace.StencilDepthFailOp = MappingDSOperation[desc.BackFace.StencilDepthFailOp];
	ds_desc.BackFace.StencilPassOp = MappingDSOperation[desc.BackFace.StencilPassOp];
	ds_desc.BackFace.StencilFunc = MappingComparisonMode[desc.BackFace.StencilFunc];

	pimpl->d3d_device->CreateDepthStencilState(&ds_desc, &ds_state);

	state->PRIVATE = (void *) ds_state;
}
	
void RendererBase::SetState(const DepthStencilState * in_state)
{
	const DepthStencilState * state = in_state ? in_state : &DefaultDepthStencil;
	pimpl->d3d_device->OMSetDepthStencilState((ID3D10DepthStencilState *) state->PRIVATE, 0);
}

void RendererBase::DestroyState(DepthStencilState * state)
{
	((ID3D10DepthStencilState *) state->PRIVATE)->Release();
	state->PRIVATE = 0;
}
	

