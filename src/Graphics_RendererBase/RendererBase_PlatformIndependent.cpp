#include "RendererBase.h"
#include "DescBlendState.h"
#include "DescDepthStencilState.h"
#include "DescRasterizerState.h"
#include "DescSamplerState.h"
#include "Interface_BlendState.h"
#include "Interface_DepthStencilState.h"
#include "Interface_RasterizerState.h"
#include "Interface_SamplerState.h"

void RendererBase::PlatformIndependentInit()
{
	next_vf_hash = 0;
	next_ss_hash = 0;
	
}

void RendererBase::CreateDefaultStates(BlendState * blend_state, DepthStencilState * depth_stencil_state, RasterizerState * rasterizer_state, SamplerState * sampler_state)
{
	DescBlendState blend_desc;
	DescRasterizerState rast_desc;
	DescDepthStencilState ds_desc;
	DescSamplerState samp_desc;

	blend_desc.SetDefaults();
	rast_desc.SetDefaults();
	ds_desc.SetDefaults();
	samp_desc.SetDefaults();

	CreateState(blend_desc, blend_state);
	CreateState(ds_desc, depth_stencil_state);
	CreateState(rast_desc, rasterizer_state);
	CreateState(samp_desc, sampler_state);
}