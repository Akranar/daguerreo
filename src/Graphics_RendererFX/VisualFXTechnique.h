#ifndef _VISUAL_FX_TECHNIQUE_H_
#define _VISUAL_FX_TECHNIQUE_H_

#include "VisualFXPass.h"
#include <Core_FixedArray.h>
//*********************************************
//	VisualFXTechnique
//*********************************************

class VisualFXTechnique
{
	FixedArray<VisualFXPass> passes;
public:
	inline void Initialize(unsigned int pass_count);

	inline VisualFXPass * GetPass(unsigned int pass);
	inline const VisualFXPass * GetPass(unsigned int pass) const;
	inline unsigned int GetPassCount() const;
	
};
inline void VisualFXTechnique::Initialize(unsigned int pass_count)
{
	passes.Set(pass_count);
}
inline VisualFXPass * VisualFXTechnique::GetPass(unsigned int pass)
{
	return &passes[pass];
}
inline const VisualFXPass * VisualFXTechnique::GetPass(unsigned int pass) const
{
	return &passes[pass];
}
inline unsigned int VisualFXTechnique::GetPassCount() const
{
	return passes.GetSize();
}



#endif