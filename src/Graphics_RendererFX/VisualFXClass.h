#ifndef _VISUAL_FX_CLASS_H_
#define _VISUAL_FX_CLASS_H_

#include "VisualFXTechnique.h"
#include <Core_FixedArray.h>

//*********************************************
//	VisualFXClass
//*********************************************

class VisualFXClass
{
	FixedArray<VisualFXTechnique> techniques;
public:
	inline void Initialize(unsigned int technique_count);
	inline VisualFXTechnique * GetTechnique(unsigned int index);
};

inline void VisualFXClass::Initialize(unsigned int technique_count)
{
	techniques.Set(technique_count);
}

inline VisualFXTechnique * VisualFXClass::GetTechnique(unsigned int index)
{
	return &techniques[index];
}



#endif