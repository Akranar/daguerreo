#include "../RendererBase.h"
#include "../DescSamplerState.h"
#include "../Interface_SamplerState.h"

#include "Mapping.h"
#include <float.h>
#include "RendererBasePimpl.h"

void RendererBase::CreateState(const DescSamplerState & desc, SamplerState * state)
{
	DescSamplerState * state_pimpl = new DescSamplerState();
	state_pimpl->SetDefaults();
	state->PRIVATE = (void *) state_pimpl;
}

/*
	float BorderColor[4];
	float MipLODBias;
	unsigned int MaxAnisotropy;
	float MinLOD;
	float MaxLOD;
	unsigned char Filter;
	unsigned char AddressU;
	unsigned char AddressV;
	unsigned char AddressW;
	unsigned char ComparisonFunc;
*/
	
void RendererBase::SetState(const SamplerState * in_state, unsigned int unit)
{
	const SamplerState * state = in_state ? in_state : &DefaultSampler;
	DescSamplerState * state_pimpl = (DescSamplerState *) state->PRIVATE;

	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_ADDRESSU, MappingSamplerEdge[state_pimpl->AddressU]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_ADDRESSV, MappingSamplerEdge[state_pimpl->AddressV]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_ADDRESSW, MappingSamplerEdge[state_pimpl->AddressW]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_BORDERCOLOR, 
		D3DCOLOR_RGBA(
		(int)(state_pimpl->BorderColor[0]*255), 
		(int)(state_pimpl->BorderColor[1]*255), 
		(int)(state_pimpl->BorderColor[2]*255), 
		(int)(state_pimpl->BorderColor[3]*255)));
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MAGFILTER, MappingSamplerFilter_MAG[state_pimpl->Filter]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MINFILTER, MappingSamplerFilter_MIN[state_pimpl->Filter]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MIPFILTER, MappingSamplerFilter_MIP[state_pimpl->Filter]);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MIPMAPLODBIAS, state_pimpl->MipLODBias);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MAXMIPLEVEL, state_pimpl->MaxLOD);
	pimpl->d3d_device->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY, state_pimpl->MaxAnisotropy);
	//pimpl->d3d_device->SetRenderState(D3DSAMP_MAXANISOTROPY, D3DTADDRESS_WRAP);
}

void RendererBase::DestroyState(SamplerState * state)
{
	DescSamplerState * state_pimpl = (DescSamplerState *) state->PRIVATE;
	delete state_pimpl;
	state->PRIVATE = 0;
}
	