#ifndef _INTERFACE_SHADER_PROGRAM_H_
#define _INTERFACE_SHADER_PROGRAM_H_

#include <Core_FixedArray.h>
#include <string>

class UniformParam
{
public:
	std::string name;
	unsigned char type;
	bool row_major;
	unsigned char rows;
	unsigned char columns;
	unsigned int array_length;
	unsigned int uniform_buffer_index;
	unsigned int uniform_buffer_offset;
	unsigned int uniform_buffer_array_stride;
	unsigned int uniform_buffer_matrix_stride;
};


class SamplerParam
{
public:
	std::string name;
	unsigned char type;
};

class UniformBlock
{
public:
	std::string name;
	unsigned int byte_count;
	FixedArray<unsigned int> uniform_indices;
};


class DescShaderSignature;

class ShaderProgram
{
public:
	void * PRIVATE;
	unsigned int signature_hash;
	DescShaderSignature * signature_desc;
	inline ShaderProgram() : PRIVATE(0), signature_hash(-1), signature_desc(0) {};

	int FindUniform(const char * name);

	//Previously ShaderDomain stuff
	FixedArray<UniformParam> uniforms;
	FixedArray<SamplerParam> samplers;
	FixedArray<UniformBlock> uniform_blocks;

	void PrintUniforms();
	void PrintBlocks();

	inline unsigned int GetUniformCount() const;
	inline unsigned int GetUniformBlockCount() const;
	inline unsigned int GetSamplerCount() const;

	inline unsigned int GetUniformBlockSize(unsigned int index) const;

	inline const std::string & GetUniformName(unsigned int index) const;
	inline unsigned char GetUniformType(unsigned int index) const;
	inline bool GetUniformRowMajor(unsigned int index) const;
	inline unsigned char GetUniformRows(unsigned int index) const;
	inline unsigned char GetUniformColumns(unsigned int index) const;
	inline unsigned int GetUniformArrayLength(unsigned int index) const;
	inline unsigned int GetUniformBufferIndex(unsigned int index) const;
	inline unsigned int GetUniformBufferOffset(unsigned int index) const;
	inline unsigned int GetUniformBufferArrayStride(unsigned int index) const;
	inline unsigned int GetUniformBufferMatrixStride(unsigned int index) const;

	inline unsigned int GetSamplerType(unsigned int index) const;
};



inline unsigned int ShaderProgram::GetUniformCount() const
{
	return uniforms.GetSize();
}
inline unsigned int ShaderProgram::GetUniformBlockCount() const
{
	return uniform_blocks.GetSize();
}
inline unsigned int ShaderProgram::GetSamplerCount() const
{
	return samplers.GetSize();
}

inline unsigned int ShaderProgram::GetUniformBlockSize(unsigned int index) const
{
	return uniform_blocks[index].byte_count;
}

inline const std::string & ShaderProgram::GetUniformName(unsigned int index) const
{
	return uniforms[index].name;
}
inline unsigned char ShaderProgram::GetUniformType(unsigned int index) const
{
	return uniforms[index].type;
}
inline bool ShaderProgram::GetUniformRowMajor(unsigned int index) const
{
	return uniforms[index].row_major;
}
inline unsigned char ShaderProgram::GetUniformRows(unsigned int index) const
{
	return uniforms[index].rows;
}
inline unsigned char ShaderProgram::GetUniformColumns(unsigned int index) const
{
	return uniforms[index].columns;
}
inline unsigned int ShaderProgram::GetUniformArrayLength(unsigned int index) const
{
	return uniforms[index].array_length;
}
inline unsigned int ShaderProgram::GetUniformBufferIndex(unsigned int index) const
{
	return uniforms[index].uniform_buffer_index;
}
inline unsigned int ShaderProgram::GetUniformBufferOffset(unsigned int index) const
{
	return uniforms[index].uniform_buffer_offset;
}
inline unsigned int ShaderProgram::GetUniformBufferArrayStride(unsigned int index) const
{
	return uniforms[index].uniform_buffer_array_stride;
}
inline unsigned int ShaderProgram::GetUniformBufferMatrixStride(unsigned int index) const
{
	return uniforms[index].uniform_buffer_matrix_stride;
}

inline unsigned int ShaderProgram::GetSamplerType(unsigned int index) const
{
	return samplers[index].type;
}




#endif