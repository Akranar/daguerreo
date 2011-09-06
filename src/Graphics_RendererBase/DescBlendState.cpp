#include "DescBlendState.h"
#include "Enumerants.h"

void DescBlendState::SetDefaults()
{
	AlphaToCoverageEnable = false;
	SrcBlend = BS_ONE;
	DestBlend = BS_ZERO;
	BlendOp = BS_FUNC_ADD;
	SrcBlendAlpha = BS_ONE;
	DestBlendAlpha = BS_ZERO;
	BlendOpAlpha = BS_FUNC_ADD;

	for (unsigned int i = 0; i < 8; ++i)
	{
		BlendEnable[i] = false;
		RenderTargetWriteMask[i][0] = true;
		RenderTargetWriteMask[i][1] = true; 
		RenderTargetWriteMask[i][2] = true; 
		RenderTargetWriteMask[i][3] = true;
	}
}

