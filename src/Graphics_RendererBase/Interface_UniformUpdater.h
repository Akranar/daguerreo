#ifndef _UNIFORM_UPDATER_H_
#define _UNIFORM_UPDATER_H_

#include <Core_FixedArray.h>
#include "DescUniformDataSource.h"

class ShaderProgram;

class UniformUpdater
{
private:
	ShaderProgram * program;
	int domain;
	UniformUpdater(const UniformUpdater & ref) {};
protected:
	FixedArray<DescUniformDataSource> data;
public:
	inline UniformUpdater() 
		: program(0), domain(-1)
	{};

	inline UniformUpdater(unsigned int count) 
	: 
	data(count), program(0), domain(-1)
	{
	};

	~UniformUpdater() 
	{ 
	};

	inline unsigned int GetDomain() const;
	inline ShaderProgram * GetShaderProgram() const;
	inline unsigned int GetParameterCount() const;
	inline DescUniformDataSource & GetDescriptor(unsigned int index);
	inline const DescUniformDataSource & GetDescriptor(unsigned int index) const;
};

inline unsigned int UniformUpdater::GetDomain() const
{
	return domain;
}
inline ShaderProgram * UniformUpdater::GetShaderProgram() const
{
	return program;
}

inline unsigned int UniformUpdater::GetParameterCount() const
{
	return data.GetSize();
}
inline DescUniformDataSource & UniformUpdater::GetDescriptor(unsigned int index)
{
	return data[index];
}
inline const DescUniformDataSource & UniformUpdater::GetDescriptor(unsigned int index) const
{
	return data[index];
}

#endif