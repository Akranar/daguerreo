#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_TextureCube.h"
#include "../Interface_ShaderProgram.h"
#include "../DescTextureCube.h"

#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"

#include <iostream>

//*************************************
//	TextureCube
//*************************************

void RendererBase::CreateResource(const DescTextureCube & desc, TextureCube * resource)
{
	//UNIMPLEMENTED
}
void RendererBase::Delete(TextureCube * resource)
{
	//UNIMPLEMENTED
}
void RendererBase::Bind(ShaderProgram * program, unsigned int unit, TextureCube * resource)
{
	//UNIMPLEMENTED
}
void * RendererBase::Map(TextureCube * resource, MapAccess access, unsigned int face)
{
	//UNIMPLEMENTED
	return 0;
}
void RendererBase::Unmap(TextureCube * resource, unsigned int face)
{
	//UNIMPLEMENTED
}