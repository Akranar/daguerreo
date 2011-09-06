/*
#ifndef _BCR_VISUAL_FX_CLASS_H_
#define _BCR_VISUAL_FX_CLASS_H_


#include "ExLib_RendererFX.h"
#include "ExLib_Serialization.h"
#include <string>

class BCR_VisualFXClass: public Serializable
{
	struct Pass
	{
		std::string program_filename;
		RenderStateList render_states;
		
	};
	struct Technique
	{
		unsigned int pass_count;
		Pass * passes;

		Technique() : pass_count(0), passes(0) {};
		~Technique()
		{
			if (passes) delete [] passes;
		}
	};
	unsigned int technique_count;
	Technique * techniques;
public:
	BCR_VisualFXClass();
	~BCR_VisualFXClass();
	void SetTechniqueCount(unsigned int count);
	void SetTechnique(unsigned int technique_index, unsigned int pass_count);
	void SetPass(unsigned int tech, unsigned int pass, const std::string & program_file, RenderStateList & src_states);

	inline unsigned int GetTechniqueCount() const;
	inline unsigned int GetPassCount(unsigned int technique_index) const;
	inline const std::string & GetProgramFile(unsigned int technique_index, unsigned int pass_index) const;
	inline const RenderStateList & GetRenderStates(unsigned int technique_index, unsigned int pass_index) const;
	inline RenderStateList & GetRenderStates(unsigned int technique_index, unsigned int pass_index);

	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	
};

inline unsigned int BCR_VisualFXClass::GetTechniqueCount() const
{
	return technique_count;
}
inline unsigned int BCR_VisualFXClass::GetPassCount(unsigned int technique_index) const
{
	return techniques[technique_index].pass_count;
}
inline const std::string & BCR_VisualFXClass::GetProgramFile(unsigned int technique_index, unsigned int pass_index) const
{
	return techniques[technique_index].passes[pass_index].program_filename;
}
inline const RenderStateList & BCR_VisualFXClass::GetRenderStates(unsigned int technique_index, unsigned int pass_index) const
{
	return techniques[technique_index].passes[pass_index].render_states;
}
inline RenderStateList & BCR_VisualFXClass::GetRenderStates(unsigned int technique_index, unsigned int pass_index)
{
	return techniques[technique_index].passes[pass_index].render_states;
}

#endif
*/