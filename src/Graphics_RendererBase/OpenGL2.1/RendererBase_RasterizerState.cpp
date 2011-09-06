#include "../RendererBase.h"
#include "../DescRasterizerState.h"
#include "../Interface_RasterizerState.h"

#include "ExLib_API_OpenGL.h"
#include "Mapping.h"

void RendererBase::CreateState(const DescRasterizerState & desc, RasterizerState * state)
{
	DescRasterizerState * pimpl = new DescRasterizerState;
	*pimpl = desc;
	state->PRIVATE = (void *) pimpl;
}
	
void RendererBase::SetState(const RasterizerState * in_state)
{
	const RasterizerState * state = in_state ? in_state : &DefaultRasterizer;
	const DescRasterizerState * rasterizer_state = ((const DescRasterizerState *) state->PRIVATE);

	glPolygonMode(GL_FRONT_AND_BACK, MappingRasterizerFill[rasterizer_state->PolygonMode]);

	if (rasterizer_state->CullFace == RS_CULL_NONE)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(MappingRasterizerCull[rasterizer_state->CullFace]);
	}
	rasterizer_state->FrontCounterClockwise ? glFrontFace(GL_CCW) : glFrontFace(GL_CW);
	glPolygonOffset(rasterizer_state->SlopeScaledDepthBias, rasterizer_state->DepthBias);

	rasterizer_state->DepthClipEnable ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
	rasterizer_state->ScissorEnable ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}
	

void RendererBase::DestroyState(RasterizerState * state)
{
	delete ((DescRasterizerState *) state->PRIVATE);
	state->PRIVATE = 0;
}
	
	