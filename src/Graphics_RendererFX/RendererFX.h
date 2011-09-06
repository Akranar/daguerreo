#ifndef _RENDERER_FX_H_
#define _RENDERER_FX_H_

//#include "ExLib_RendererBase.h"

template <class BASE_RENDERER>
class RendererFX : public BASE_RENDERER
{
public:
	void BindFXPass(VisualFXInstance * fx, unsigned int pass_index);
};

//Requires:
//	-> SetState(BlendState *)
//	-> SetState(DepthStencilState *)
//	-> SetState(RasterizerState *)
//	-> SetState(SamplerSamplerState *, domain_index, sampler_index)
//	-> Bind(ShaderProgram *)
//	-> Bind(UniformBuffer *)
//	-> Bind(Texture1D *)
//	-> Bind(Texture2D *)
//	-> Bind(Texture3D *)
//	-> Bind(TextureCube *)
template <class BASE_RENDERER>
void RendererFX<BASE_RENDERER>::BindFXPass(VisualFXInstance * fx, unsigned int pass_index)
{
	VisualFXPass * pass = fx->GetTechnique()->GetPass(pass_index);
	ShaderProgram * shader_program = pass->GetShaderProgram();
	BASE_RENDERER::BindShaderProgram(shader_program);
	BASE_RENDERER::SetState(pass->GetBlendState());
	BASE_RENDERER::SetState(pass->GetDepthStencilState());
	BASE_RENDERER::SetState(pass->GetRasterizerState());
	for (unsigned int uniform_index = 0; uniform_index < shader_program->GetUniformBlockCount(); ++uniform_index)
	{
		BASE_RENDERER::BindUniformBuffer(fx->GetUniformBuffer(pass_index, uniform_index), shader_program);
	}
	for (unsigned int sampler_index = 0; sampler_index < shader_program->GetSamplerCount(); ++sampler_index)
	{
		Texture * texture = fx->GetTexture(pass_index, sampler_index);
		switch(shader_program->GetSamplerType(sampler_index))
		{
			case ST_SAMPLER_1D:
			{
				//BASE_RENDERER::Bind(shader_program, domain, sampler_index, texture->Cast1D());
			}
			break;
			case ST_SAMPLER_2D:
			{
				BASE_RENDERER::Bind(shader_program, sampler_index, texture->Cast2D());
			}
			break;
			case ST_SAMPLER_3D:
			{
				//BASE_RENDERER::Bind(shader_program, domain, sampler_index, texture->Cast3D());
			}
			break;
			case ST_SAMPLER_CUBE:
			{
				BASE_RENDERER::Bind(shader_program, sampler_index, texture->CastCube());
			}
			break;
			default:
			break;
		}
		BASE_RENDERER::SetState(pass->GetSamplerState(sampler_index), sampler_index);
	}
}

#endif