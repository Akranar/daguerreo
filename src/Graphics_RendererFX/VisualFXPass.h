#ifndef _VISUAL_FX_PASS_H_
#define _VISUAL_FX_PASS_H_

#include <Core_FixedArray.h>
#include "ExLib_RendererBase.h"

class VisualFXPass
{
	ShaderProgram * shader_program;
	BlendState * blend_state;
	DepthStencilState * depth_stencil_state;
	RasterizerState * rasterizer_state;
	FixedArray<SamplerState *> sampler_states;
public:
	inline VisualFXPass();
	inline void Initialize(ShaderProgram * in_shader_program);
	inline void SetRenderStates(BlendState * in_blend_state, DepthStencilState * in_depth_stencil_state, RasterizerState * in_rast_state);
	inline void SetSamplerState(unsigned int unit, SamplerState * in_sampler_state);
	inline BlendState * GetBlendState() const;
	inline DepthStencilState * GetDepthStencilState() const;
	inline RasterizerState * GetRasterizerState() const;
	inline SamplerState * GetSamplerState(unsigned int index) const;
	inline unsigned int GetSamplerStateCount() const;

	inline ShaderProgram * GetShaderProgram() const;
};

inline VisualFXPass::VisualFXPass()
:
shader_program(0),
blend_state(0),
depth_stencil_state(0),
rasterizer_state(0)
{

}

inline void VisualFXPass::Initialize(ShaderProgram * in_shader_program)
{
	shader_program = in_shader_program;
	sampler_states.Set(in_shader_program->GetSamplerCount(), 0);
}
inline void VisualFXPass::SetRenderStates(BlendState * in_blend_state, DepthStencilState * in_depth_stencil_state, RasterizerState * in_rast_state)
{
	blend_state = in_blend_state;
	depth_stencil_state = in_depth_stencil_state;
	rasterizer_state = in_rast_state;
}
inline void VisualFXPass::SetSamplerState(unsigned int unit, SamplerState * in_sampler_state)
{
	sampler_states[unit] = in_sampler_state;
}
inline BlendState * VisualFXPass::GetBlendState() const
{
	return blend_state;
}
inline DepthStencilState * VisualFXPass::GetDepthStencilState() const
{
	return depth_stencil_state;
}
inline RasterizerState * VisualFXPass::GetRasterizerState() const
{
	return rasterizer_state;
}
inline SamplerState * VisualFXPass::GetSamplerState(unsigned int unit) const
{
	return sampler_states[unit];
}

inline unsigned int VisualFXPass::GetSamplerStateCount() const
{
	return sampler_states.GetSize();
}

inline ShaderProgram * VisualFXPass::GetShaderProgram() const
{
	return shader_program;
}


#endif