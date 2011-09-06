#include "VisualFXInstance.h"
#include "ExLib_RendererBase.h"


void VisualFXInstance::Initialize(VisualFXClass * in_class, unsigned int _technique_index)
{
	fx_class = in_class;
	technique_index = _technique_index;
	unsigned int pass_count = fx_class->GetTechnique(technique_index)->GetPassCount();
	uniform_buffers.Set(pass_count);
	textures.Set(pass_count);
	for (unsigned int pass_index = 0; pass_index < pass_count; ++pass_index)
	{
		VisualFXPass * pass = fx_class->GetTechnique(technique_index)->GetPass(pass_index);
		unsigned int uniform_count = pass->GetShaderProgram()->GetUniformBlockCount();
		unsigned int sampler_count = pass->GetShaderProgram()->GetSamplerCount();
		uniform_buffers[pass_index].Set(uniform_count, 0);
		textures[pass_index].Set(sampler_count, 0);
		
	}
}