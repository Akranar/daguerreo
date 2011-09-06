#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../DescShaderProgram.h"

#include <iostream>

#include "ShaderProgramPimpl.h"
#include "ShaderReflectionPimpl.h"

//*************************************
//	ShaderProgram
//*************************************

#include <algorithm>

void RendererBase::CreateShaderReflection(ShaderProgram * resource, ShaderReflection * reflection)
{
	/*
	ShaderProgramPimpl * resource_pimpl = (ShaderProgramPimpl *) resource->PRIVATE;
	//GLuint program = resource_pimpl->combined;


	ShaderReflectionPimpl * reflection_pimpl = new ShaderReflectionPimpl();
	reflection->PRIVATE = (void *) reflection_pimpl;
	*/
}
void RendererBase::DeleteShaderReflection(ShaderReflection * reflection)
{
	/*
	ShaderReflectionPimpl * reflection_pimpl = (ShaderReflectionPimpl *) reflection->PRIVATE;
	delete reflection_pimpl;
	reflection->PRIVATE = 0;
	*/
}