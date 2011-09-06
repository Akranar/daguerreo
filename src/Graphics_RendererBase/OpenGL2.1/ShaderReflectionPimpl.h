#ifndef _SHADER_REFLECTION_PIMPL_H_
#define _SHADER_REFLECTION_PIMPL_H_

#include "ExLib_API_OpenGL.h"

#include <vector>
#include <string>

class ShaderReflectionPimpl
{
public:
	std::vector<std::string> uniform_names;
	std::vector<GLuint> uniform_locations;
	std::vector<GLuint> uniform_types;
	std::vector<GLuint> uniform_sizes;

	std::vector<std::string> attrib_names;
	std::vector<GLuint> attrib_locations;
	std::vector<GLuint> attrib_types;
	std::vector<GLuint> attrib_sizes;

	std::vector<std::string> sampler_names;
	std::vector<GLuint> sampler_locations;
	std::vector<GLuint> sampler_types;
	std::vector<GLuint> sampler_units;
	std::vector<GLuint> sampler_sizes;
	

	void PrintUniformParams(GLuint program);
	void PrintAttributes(GLuint program);
	void PrintUniformBuffers(GLuint program);
};

#endif