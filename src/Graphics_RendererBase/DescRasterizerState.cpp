#include "DescRasterizerState.h"
#include "Enumerants.h"

void DescRasterizerState::SetDefaults()
{
	ScissorRect[0] = 0;
	ScissorRect[1] = 0;
	ScissorRect[2] = 0;
	ScissorRect[3] = 0;
	PolygonMode = RS_FILL_SOLID;
	CullFace = RS_CULL_BACK;
	FrontCounterClockwise = true;
	DepthBias = 0.0f;
	SlopeScaledDepthBias = 0.0f;
	DepthClipEnable = true;
	ScissorEnable = false;
}


