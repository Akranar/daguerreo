#include "../RendererBase.h"
#include "../DescSamplerState.h"
#include "../Interface_SamplerState.h"

#include "ExLib_API_OpenGL.h"
#include "Mapping.h"

#include "RendererBasePimpl.h"
#include "../Interface_ShaderProgram.h"

void RendererBase::CreateState(const DescSamplerState & desc, SamplerState * state)
{
	DescSamplerState * pimpl = new DescSamplerState;
	*pimpl = desc;
	state->PRIVATE = (void *) pimpl;
}
	
void RendererBase::SetState(const SamplerState * in_state, unsigned int unit)
{
	if (!pimpl->bound_shader_program) return;

	const SamplerState * state = in_state ? in_state : &DefaultSampler;

	unsigned int sampler_type = pimpl->bound_shader_program->GetSamplerType(unit);
	GLuint gl_sampler_type = MappingSamplerType[sampler_type];

	const DescSamplerState * sampler_state = ((const DescSamplerState *) state->PRIVATE);

	GLuint comparison_func = MappingDSCompare[sampler_state->ComparisonFunc];
	GLuint wrap_s = MappingSamplerEdge[sampler_state->AddressU];
	GLuint wrap_t = MappingSamplerEdge[sampler_state->AddressV];
	GLuint wrap_r = MappingSamplerEdge[sampler_state->AddressW];

	glTexParameteri(gl_sampler_type, GL_TEXTURE_MAG_FILTER, MappingSamplerFilter_MAG[sampler_state->Filter]);
	glTexParameteri(gl_sampler_type, GL_TEXTURE_MIN_FILTER, MappingSamplerFilter_MIN_MIP[sampler_state->Filter]);

	switch (gl_sampler_type)
	{
	case GL_TEXTURE_3D:
		glTexParameteri(gl_sampler_type, GL_TEXTURE_WRAP_R, wrap_r);
	case GL_TEXTURE_CUBE_MAP:
	case GL_TEXTURE_2D:
		glTexParameteri(gl_sampler_type, GL_TEXTURE_WRAP_T, wrap_t);
	case GL_TEXTURE_1D:
		glTexParameteri(gl_sampler_type, GL_TEXTURE_WRAP_S, wrap_s);
	}
	
	
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, sampler_state->MipLODBias);
	glTexParameteri(gl_sampler_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler_state->MaxAnisotropy);
	glTexParameteri(gl_sampler_type, GL_TEXTURE_COMPARE_FUNC, comparison_func);
	glTexParameterfv(gl_sampler_type, GL_TEXTURE_BORDER_COLOR, sampler_state->BorderColor);
	glTexParameterf(gl_sampler_type, GL_TEXTURE_MIN_LOD, sampler_state->MinLOD);
	glTexParameterf(gl_sampler_type, GL_TEXTURE_MAX_LOD, sampler_state->MaxLOD);
}

void RendererBase::DestroyState(SamplerState * state)
{
	delete ((DescSamplerState *) state->PRIVATE);
	state->PRIVATE = 0;
}
	