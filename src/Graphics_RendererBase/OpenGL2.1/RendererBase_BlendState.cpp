#include "../RendererBase.h"
#include "../DescBlendState.h"
#include "../Interface_BlendState.h"

#include "Mapping.h"

void RendererBase::CreateState(const DescBlendState & desc, BlendState * state)
{
	DescBlendState * pimpl = new DescBlendState;
	*pimpl = desc;
	state->PRIVATE = (void *) pimpl;
}
	
void RendererBase::SetState(const BlendState * in_blend_state)
{
	const BlendState * blend_state = in_blend_state ? in_blend_state : &DefaultBlend;

	//glSampleMaski(0, -1);
	const DescBlendState * state = (const DescBlendState *) blend_state->PRIVATE;
	state->AlphaToCoverageEnable ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	/*
	for (unsigned int i = 0; i < 8; ++i)
	{
		state->BlendEnable[i] ? glEnablei(GL_BLEND, i) : glDisablei(GL_BLEND, i);

		glColorMaski(i, 
			state->RenderTargetWriteMask[i][0], 
			state->RenderTargetWriteMask[i][1], 
			state->RenderTargetWriteMask[i][2], 
			state->RenderTargetWriteMask[i][3]);
	}
	*/
	state->BlendEnable[0] ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

	glColorMask(
		state->RenderTargetWriteMask[0][0], 
		state->RenderTargetWriteMask[0][1], 
		state->RenderTargetWriteMask[0][2], 
		state->RenderTargetWriteMask[0][3]);
	
	glBlendFuncSeparate(
		MappingBlendFactor[state->SrcBlend], 
		MappingBlendFactor[state->DestBlend], 
		MappingBlendFactor[state->SrcBlendAlpha], 
		MappingBlendFactor[state->DestBlendAlpha]);
	glBlendEquationSeparate(MappingBlendFunc[state->BlendOp], MappingBlendFunc[state->BlendOpAlpha]);

	glDisable(GL_ALPHA_TEST);
}
	
void RendererBase::DestroyState(BlendState * state)
{
	delete ((DescBlendState *) state->PRIVATE);
	state->PRIVATE = 0;
}
	