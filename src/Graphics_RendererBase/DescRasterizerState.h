#ifndef _DESC_RASTERIZER_STATE_H_
#define _DESC_RASTERIZER_STATE_H_

#include "ExLib_Serialization.h"

class DescRasterizerState
{
public:
	int ScissorRect[4];
	float DepthBias;
	float SlopeScaledDepthBias;
	unsigned char PolygonMode;
	unsigned char CullFace;
	bool FrontCounterClockwise;
	bool DepthClipEnable;
	bool ScissorEnable;

	void SetDefaults();
};



#endif