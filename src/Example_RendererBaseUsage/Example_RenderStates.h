#ifndef _EXAMPLE_RENDER_STATES_H_
#define _EXAMPLE_RENDER_STATES_H_

#include "ExLib_RendererBase.h"

class Example_RenderStates
{
public:
	BlendState blend_state;
	RasterizerState rast_state;
	DepthStencilState ds_state;
	SamplerState sampler_state;

	RendererBase * renderer;

	void Init(RendererBase * _renderer)
	{
		renderer = _renderer;
		DescBlendState blend_desc;
		DescRasterizerState rast_desc;
		DescDepthStencilState ds_desc;
		DescSamplerState samp_desc;

		blend_desc.SetDefaults();
		rast_desc.SetDefaults();
		ds_desc.SetDefaults();
		samp_desc.SetDefaults();

		renderer->CreateState(blend_desc, &blend_state);
		renderer->CreateState(rast_desc, &rast_state);
		renderer->CreateState(ds_desc, &ds_state);
		renderer->CreateState(samp_desc, &sampler_state);
	};
	void Set()
	{
		renderer->SetState(&blend_state);
		renderer->SetState(&rast_state);
		renderer->SetState(&ds_state);
		renderer->SetState(&sampler_state, 0);
	}
	void Deinit()
	{
		renderer->DestroyState(&blend_state);
		renderer->DestroyState(&rast_state);
		renderer->DestroyState(&ds_state);
		renderer->DestroyState(&sampler_state);
	}
};

#endif