#include "../RendererBase.h"
#include "../DescSamplerState.h"
#include "../Interface_SamplerState.h"

#include "Mapping.h"
#include <float.h>
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescSamplerState & desc, SamplerState * state)
{
	D3D10_SAMPLER_DESC samp_desc;
	ID3D10SamplerState * samp_state;

	samp_desc.Filter = MappingSamplerFilter[desc.Filter];
	samp_desc.AddressU = MappingSamplerEdge[desc.AddressU];
	samp_desc.AddressV = MappingSamplerEdge[desc.AddressV];
	samp_desc.AddressW = MappingSamplerEdge[desc.AddressW];
	samp_desc.MipLODBias = desc.MipLODBias;
	samp_desc.MaxAnisotropy = desc.MaxAnisotropy;
	samp_desc.ComparisonFunc = MappingComparisonMode[desc.ComparisonFunc];
	samp_desc.BorderColor[0] = desc.BorderColor[0];
	samp_desc.BorderColor[1] = desc.BorderColor[1];
	samp_desc.BorderColor[2] = desc.BorderColor[2];
	samp_desc.BorderColor[3] = desc.BorderColor[3];
	samp_desc.MinLOD = desc.MinLOD;
	samp_desc.MaxLOD = desc.MaxLOD;

	pimpl->d3d_device->CreateSamplerState(&samp_desc, &samp_state);

	state->PRIVATE = (void *) samp_state;
}
	
void RendererBase::SetState(const SamplerState * in_state, unsigned int unit)
{
	/*
	const SamplerState * state = in_state ? in_state : &DefaultSampler;
	ID3D10SamplerState * samp_state = ((ID3D10SamplerState *) state->PRIVATE);
	pimpl->d3d_device->VSSetSamplers(unit, 1, &samp_state);
	*/
}

void RendererBase::DestroyState(SamplerState * state)
{
	((ID3D10SamplerState *) state->PRIVATE)->Release();
	state->PRIVATE = 0;
}
	