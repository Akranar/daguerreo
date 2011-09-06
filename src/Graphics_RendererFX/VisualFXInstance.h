#ifndef _VISUAL_FX_INSTANCE_H_
#define _VISUAL_FX_INSTANCE_H_

#include "VisualFXPass.h"
#include "VisualFXTechnique.h"
#include "VisualFXClass.h"
#include <Core_FixedArray.h>

//*********************************************
//	VisualFXInstance
//*********************************************

class UniformBuffer;
class Texture;

class VisualFXInstance
{
	unsigned short technique_index;
	VisualFXClass * fx_class;
	FixedArray<FixedArray<UniformBuffer *> > uniform_buffers;
	FixedArray<FixedArray<Texture *> > textures;
public:
	void Initialize(VisualFXClass * in_class, unsigned int _technique_index);

	inline void SetUniformBuffer(unsigned int pass, unsigned int index, UniformBuffer * buffer);
	inline void SetTexture(unsigned int pass, unsigned int index, Texture * texture);

	inline VisualFXTechnique * GetTechnique() const;
	inline UniformBuffer * GetUniformBuffer(unsigned int pass, unsigned int index);
	inline Texture * GetTexture(unsigned int pass, unsigned int index);
};



inline void VisualFXInstance::SetUniformBuffer(unsigned int pass, unsigned int index, UniformBuffer * buffer)
{
	uniform_buffers[pass][index] = buffer;
}
inline void VisualFXInstance::SetTexture(unsigned int pass, unsigned int index, Texture * texture)
{
	textures[pass][index] = texture;
}

inline VisualFXTechnique * VisualFXInstance::GetTechnique() const
{
	return fx_class->GetTechnique(technique_index);
}
inline UniformBuffer * VisualFXInstance::GetUniformBuffer(unsigned int pass, unsigned int index)
{
	return uniform_buffers[pass][index];
}
inline Texture * VisualFXInstance::GetTexture(unsigned int pass, unsigned int index)
{
	return textures[pass][index];
}

#endif