#include "../RendererBase.h"
#include "../DescRasterizerState.h"
#include "../Interface_RasterizerState.h"


#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescRasterizerState & desc, RasterizerState * state)
{
	D3D10_RASTERIZER_DESC rast_desc;
	ID3D10RasterizerState * rast_state;

	rast_desc.FillMode = MappingRSFill[desc.PolygonMode];
	rast_desc.CullMode = MappingRSCull[desc.CullFace];
	rast_desc.FrontCounterClockwise = desc.FrontCounterClockwise;
	rast_desc.DepthBias = desc.DepthBias;
	rast_desc.DepthBiasClamp = 0.0f; //Not supported.
	rast_desc.SlopeScaledDepthBias = desc.SlopeScaledDepthBias;
	rast_desc.DepthClipEnable = desc.DepthClipEnable;
	rast_desc.ScissorEnable = desc.ScissorEnable;
	rast_desc.MultisampleEnable = FALSE; //
	rast_desc.AntialiasedLineEnable = FALSE; //

	pimpl->d3d_device->CreateRasterizerState(&rast_desc, &rast_state);

	state->PRIVATE = (void *) rast_state;
	
}
	
void RendererBase::SetState(const RasterizerState * in_state)
{
	const RasterizerState * state = in_state ? in_state : &DefaultRasterizer;
	pimpl->d3d_device->RSSetState((ID3D10RasterizerState *) state->PRIVATE);
}
	

void RendererBase::DestroyState(RasterizerState * state)
{
	((ID3D10RasterizerState *) state->PRIVATE)->Release();
	state->PRIVATE = 0;
}
	
