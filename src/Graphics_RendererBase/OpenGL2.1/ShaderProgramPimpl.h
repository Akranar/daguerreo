#ifndef _SHADER_PROGRAM_PIMPL_H_
#define _SHADER_PROGRAM_PIMPL_H_

#include "ExLib_API_OpenGL.h"

#include <Core_FixedArray.h>

class UniformBuffer;

class ShaderDomainPimpl
{
public:/*
	FixedArray<GLuint> uniform_locations;
	FixedArray<GLuint> sampler_locations;
	FixedArray<GLuint> sampler_units;
	FixedArray<GLuint> block_bind_points;
	FixedArray<UniformBuffer*> bound_uniform_buffers;
	*/
};

class ShaderProgramPimpl
{
friend class RendererBase;
friend class RendererBaseEntries;
	GLuint vertex;
	GLuint fragment;
	GLuint combined;
public:
	FixedArray<GLuint> uniform_locations;
	FixedArray<GLuint> sampler_locations;
	FixedArray<GLuint> sampler_units;
	FixedArray<GLuint> block_bind_points;
	FixedArray<UniformBuffer*> bound_uniform_buffers;
	
};

#endif