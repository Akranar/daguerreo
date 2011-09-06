#include "DescSamplerState.h"
#include <float.h>
#include "Enumerants.h"

void DescSamplerState::SetDefaults()
{
	Filter = FILTER_NEAREST_NEAREST_NEAREST;
	AddressU = SAMPLER_EDGE_WRAP;
	AddressV = SAMPLER_EDGE_WRAP;
	AddressW = SAMPLER_EDGE_WRAP;
	MipLODBias = 0.0f;
	MaxAnisotropy = 16;
	ComparisonFunc = COMPARE_NEVER;
	BorderColor[0] = 0.0f;
	BorderColor[1] = 0.0f;
	BorderColor[2] = 0.0f;
	BorderColor[3] = 0.0f;
	MinLOD = 0.0f;
	MaxLOD = FLT_MAX;
}