#include "../RendererBase.h"
#include "../DescDepthStencilState.h"
#include "../Interface_DepthStencilState.h"

#include "ExLib_API_OpenGL.h"
#include "Mapping.h"

void RendererBase::CreateState(const DescDepthStencilState & desc, DepthStencilState * state)
{
	DescDepthStencilState * pimpl = new DescDepthStencilState;
	*pimpl = desc;
	state->PRIVATE = (void *) pimpl;
}
	
void RendererBase::SetState(const DepthStencilState * in_state)
{
	const DepthStencilState * state = in_state ? in_state : &DefaultDepthStencil;
	const DescDepthStencilState * depthstencil_state = ((const DescDepthStencilState *) state->PRIVATE);

	glStencilOpSeparate(GL_FRONT, 
		MappingStencilOp[depthstencil_state->FrontFace.StencilFailOp], 
		MappingStencilOp[depthstencil_state->FrontFace.StencilDepthFailOp],
		MappingStencilOp[depthstencil_state->FrontFace.StencilPassOp]);

	glStencilOpSeparate(GL_BACK, 
		MappingStencilOp[depthstencil_state->BackFace.StencilFailOp], 
		MappingStencilOp[depthstencil_state->BackFace.StencilDepthFailOp],
		MappingStencilOp[depthstencil_state->BackFace.StencilPassOp]);

	depthstencil_state->DepthEnable ?  glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	depthstencil_state->StencilEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	depthstencil_state->DepthWriteMask ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
	glDepthFunc(MappingDSCompare[depthstencil_state->DepthFunc]);
	
	glStencilFuncSeparate(GL_FRONT, 
		MappingDSCompare[depthstencil_state->FrontFace.StencilFunc], 
		depthstencil_state->StencilReference, depthstencil_state->StencilReadMask);
	glStencilFuncSeparate(GL_BACK, 
		MappingDSCompare[depthstencil_state->BackFace.StencilFunc],
		depthstencil_state->StencilReference, depthstencil_state->StencilReadMask);

	glStencilMask(depthstencil_state->StencilWriteMask);
}

void RendererBase::DestroyState(DepthStencilState * state)
{
	delete ((DescDepthStencilState *) state->PRIVATE);
	state->PRIVATE = 0;
}
	
	
	
	