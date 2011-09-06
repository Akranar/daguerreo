#ifndef _DESC_SAMPLER_STATE_H_
#define _DESC_SAMPLER_STATE_H_

#include "ExLib_Serialization.h"

class DescSamplerState
{
public:
	
	
//protected:
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

	void SetDefaults();
};


#endif