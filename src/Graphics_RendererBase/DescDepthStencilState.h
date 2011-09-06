#ifndef _DESC_DEPTH_STENCIL_STATE_H_
#define _DESC_DEPTH_STENCIL_STATE_H_

#include "ExLib_Serialization.h"

class DescDepthStencilState
{
public:
	class FaceStencilDesc
	{
	public:
		unsigned char StencilFailOp;
		unsigned char StencilDepthFailOp;
		unsigned char StencilPassOp;
		unsigned char StencilFunc;
	};

	unsigned int StencilReadMask;
	unsigned int StencilWriteMask;
	unsigned int StencilReference;
	unsigned char DepthFunc;
	FaceStencilDesc FrontFace;
	FaceStencilDesc BackFace;
	bool DepthEnable;
	bool DepthWriteMask;
	bool StencilEnable;

	void SetDefaults();
};



#endif