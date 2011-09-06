#ifndef _BCR_VISUAL_FX_INSTANCE_H_
#define _BCR_VISUAL_FX_INSTANCE_H_

#include "ExLib_RendererFX.h"
#include "ExLib_Serialization.h"
#include <string>
#include <vector>

class BCR_VisualFXInstance: public Serializable
{
public:
	enum TextureType
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE,
		TEXTURE_RECT
	};
	enum UniformDataType
	{
		DOUBLE,
		FLOAT,
		INT
	};
private:
	class TextureFile: public Serializable
	{
	public:
		int type;
		std::string filename;
		//Serialization
		virtual void Serialize(Stream & stream) const;
		virtual void Unserialize(Stream & stream);
		virtual void SerializeTypeId(Stream & stream) const;
	};
	class UniformParam: public Serializable
	{
	public:
		unsigned int rows, columns;
		bool column_major;
		int type;
		union 
		{
			double * double_data;
			float * float_data;
			int * int_data;
		} data;
		//Serialization
		virtual void Serialize(Stream & stream) const;
		virtual void Unserialize(Stream & stream);
		virtual void SerializeTypeId(Stream & stream) const;
	};
	class Domain: public Serializable
	{
	public:
		std::vector<TextureFile> texture_files;
		std::vector<UniformParam> uniform_params;
		//Serialization
		virtual void Serialize(Stream & stream) const;
		virtual void Unserialize(Stream & stream);
		virtual void SerializeTypeId(Stream & stream) const;
	};
	class Pass: public Serializable
	{
	public:
		std::vector<Domain> domains;
		//Serialization
		virtual void Serialize(Stream & stream) const;
		virtual void Unserialize(Stream & stream);
		virtual void SerializeTypeId(Stream & stream) const;
	};

	std::string class_file;
	unsigned int technique_index;
	std::vector<Pass> passes;
public:
	BCR_VisualFXInstance();
	~BCR_VisualFXInstance();

	inline const std::string & GetClassFile() const;
	inline void SetClassFile(const std::string & in_class_file);

	inline unsigned int GetTechniqueIndex() const;
	inline void SetTechniqueIndex(unsigned int index);

	inline unsigned int GetPassCount() const;
	inline unsigned int GetDomainCount(unsigned int pass_index) const;
	inline unsigned int GetTextureCount(unsigned int pass_index, unsigned int domain_index) const;
	inline unsigned int GetTextureType(unsigned int pass_index, unsigned int domain_index) const;
	inline const std::string & GetTextureFile(unsigned int pass_index, unsigned int domain_index, unsigned int texture_index) const;
	inline unsigned int GetParamCount(unsigned int pass_index, unsigned int domain_index, unsigned int texture_index) const;

	void SetPassCount(unsigned int in_pass_count);
	void SetDomainCount(unsigned int pass_index, unsigned int domain_count);
	void SetTextureCount(unsigned int pass_index, unsigned int domain_index, unsigned int texture_count);
	void SetParamCount(unsigned int pass_index, unsigned int domain_index, unsigned int param_count);
	void SetTextureFile(unsigned int pass_index, unsigned domain_index, unsigned int texture_index, const std::string & in_filename);
	void SetTextureType(unsigned int pass_index, unsigned domain_index, unsigned int texture_index, TextureType type);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline const std::string & BCR_VisualFXInstance::GetClassFile() const
{
	return class_file;
}
inline void BCR_VisualFXInstance::SetClassFile(const std::string & in_class_file)
{
	class_file = in_class_file;
}
inline unsigned int BCR_VisualFXInstance::GetTechniqueIndex() const
{
	return technique_index;
}
inline void BCR_VisualFXInstance::SetTechniqueIndex(unsigned int index)
{
	technique_index = index;
}
inline unsigned int BCR_VisualFXInstance::GetPassCount() const
{
	return passes.size();
}
inline unsigned int BCR_VisualFXInstance::GetDomainCount(unsigned int pass_index) const
{
	return passes[pass_index].domains.size();
}
inline unsigned int BCR_VisualFXInstance::GetTextureCount(unsigned int pass_index, unsigned int domain_index) const
{
	return passes[pass_index].domains[domain_index].texture_files.size();
}
inline unsigned int BCR_VisualFXInstance::GetTextureType(unsigned int pass_index, unsigned int domain_index) const
{
	return passes[pass_index].domains[domain_index].uniform_params.size();
}
inline const std::string & BCR_VisualFXInstance::GetTextureFile(unsigned int pass_index, unsigned int domain_index, unsigned int texture_index) const
{
	return passes[pass_index].domains[domain_index].texture_files[texture_index].filename;
}
inline unsigned int BCR_VisualFXInstance::GetParamCount(unsigned int pass_index, unsigned int domain_index, unsigned int texture_index) const
{
	return passes[pass_index].domains[domain_index].texture_files[texture_index].type;
}

#endif