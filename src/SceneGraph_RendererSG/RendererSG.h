#ifndef _RENDERER_SG_H_
#define _RENDERER_SG_H_

#include "ExLib_RendererFX.h"
#include "ExLib_SceneBase.h"

template <class BASE_RENDERER>
class RendererSG: public BASE_RENDERER
{
public:
	void DrawVisual(Visual * visual);

};

template <class BASE_RENDERER>
void RendererSG<BASE_RENDERER>::DrawVisual(Visual * visual)
{
	
	BASE_RENDERER::BindVertexBuffer(visual->GetVertexBuffer());
	BASE_RENDERER::BindIndexBuffer(visual->GetIndexBuffer());
	VisualFXInstance * fx_instance = visual->GetFX();
	for (unsigned int i = 0; i < fx_instance->GetTechnique()->GetPassCount(); ++i)
	{
		BASE_RENDERER::BindFXPass(fx_instance, i);
		VisualFXPass * fx_pass = fx_instance->GetTechnique()->GetPass(i);
		BASE_RENDERER::SetInputLayout(visual->GetVertexBuffer()->format_hash, fx_pass->GetShaderProgram()->signature_hash);
		BASE_RENDERER::Draw();
	}
}

#endif