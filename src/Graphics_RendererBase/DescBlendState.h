#ifndef _DESC_BLEND_STATE_H_
#define _DESC_BLEND_STATE_H_

#include "ExLib_Serialization.h"

class DescBlendState
{
public:
	bool BlendEnable[8];
	bool RenderTargetWriteMask[8][4];
	bool AlphaToCoverageEnable;
	unsigned char SrcBlend;
	unsigned char DestBlend;
	unsigned char BlendOp;
	unsigned char SrcBlendAlpha;
	unsigned char DestBlendAlpha;
	unsigned char BlendOpAlpha;

	void SetDefaults();
};



#endif