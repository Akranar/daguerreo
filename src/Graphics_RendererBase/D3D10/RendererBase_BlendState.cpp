#include "../RendererBase.h"
#include "../DescBlendState.h"
#include "../Interface_BlendState.h"

#include "Mapping.h"
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescBlendState & desc, BlendState * state)
{
	D3D10_BLEND_DESC blend_desc;
	ID3D10BlendState * blend_state;

	blend_desc.AlphaToCoverageEnable = desc.AlphaToCoverageEnable;
	blend_desc.BlendEnable[0] = desc.BlendEnable[0];
	blend_desc.BlendEnable[1] = desc.BlendEnable[1];
	blend_desc.BlendEnable[2] = desc.BlendEnable[2];
	blend_desc.BlendEnable[3] = desc.BlendEnable[3];
	blend_desc.BlendEnable[4] = desc.BlendEnable[4];
	blend_desc.BlendEnable[5] = desc.BlendEnable[5];
	blend_desc.BlendEnable[6] = desc.BlendEnable[6];
	blend_desc.BlendEnable[7] = desc.BlendEnable[7];
	blend_desc.SrcBlend = MappingBlendFactor[desc.SrcBlend];
	blend_desc.DestBlend = MappingBlendFactor[desc.DestBlend];
	blend_desc.BlendOp = MappingBlendOp[desc.BlendOp];
	blend_desc.SrcBlendAlpha = MappingBlendFactor[desc.SrcBlendAlpha];
	blend_desc.DestBlendAlpha = MappingBlendFactor[desc.DestBlendAlpha];
	blend_desc.BlendOpAlpha = MappingBlendOp[desc.BlendOpAlpha];
	for (unsigned int i = 0; i < 8; ++i)
	{
		blend_desc.RenderTargetWriteMask[i] = 0;
		if (desc.RenderTargetWriteMask[i][0])
		{
			blend_desc.RenderTargetWriteMask[i] |= D3D10_COLOR_WRITE_ENABLE_RED;
		}
		if (desc.RenderTargetWriteMask[i][1])
		{
			blend_desc.RenderTargetWriteMask[i] |= D3D10_COLOR_WRITE_ENABLE_GREEN;
		}
		if (desc.RenderTargetWriteMask[i][2])
		{
			blend_desc.RenderTargetWriteMask[i] |= D3D10_COLOR_WRITE_ENABLE_BLUE;
		}
		if (desc.RenderTargetWriteMask[i][3])
		{
			blend_desc.RenderTargetWriteMask[i] |= D3D10_COLOR_WRITE_ENABLE_ALPHA;
		}
	}
	pimpl->d3d_device->CreateBlendState(&blend_desc, &blend_state);
	
	state->PRIVATE = (void *) blend_state;
}
	
void RendererBase::SetState(const BlendState * in_blend_state)
{
	const BlendState * blend_state = in_blend_state ? in_blend_state : &DefaultBlend;
	FLOAT blend_factors[4];
	blend_factors[0] = 0;
	blend_factors[1] = 0;
	blend_factors[2] = 0;
	blend_factors[3] = 0;
	pimpl->d3d_device->OMSetBlendState((ID3D10BlendState *) blend_state->PRIVATE, blend_factors, 0xffffffff);
}
	
void RendererBase::DestroyState(BlendState * state)
{
	((ID3D10BlendState *) state->PRIVATE)->Release();
	state->PRIVATE = 0;
}
	


