#include "DescDepthStencilState.h"
#include "Enumerants.h"


void DescDepthStencilState::SetDefaults()
{
	StencilReference = -1;
	DepthEnable = true;
	DepthWriteMask = true;
	DepthFunc = COMPARE_LEQUAL;
	StencilEnable = false;
	StencilReadMask = -1;
	StencilWriteMask = -1;
	FrontFace.StencilFailOp = DS_OP_KEEP;
	FrontFace.StencilDepthFailOp = DS_OP_KEEP;
	FrontFace.StencilPassOp = DS_OP_KEEP;
	FrontFace.StencilFunc = COMPARE_ALWAYS;
	BackFace.StencilFailOp = DS_OP_KEEP;
	BackFace.StencilDepthFailOp = DS_OP_KEEP;
	BackFace.StencilPassOp = DS_OP_KEEP;
	BackFace.StencilFunc = COMPARE_ALWAYS;
}

