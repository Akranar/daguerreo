#include "../RendererBase.h"
#include "../Interface_ShaderProgram.h"
#include "../Interface_UniformBuffer.h"
#include "../DescUniformBuffer.h"


#include "ShaderProgramPimpl.h"
#include "RendererBasePimpl.h"

#include <Core_Exception.h>


void RendererBase::CreateResource(const DescUniformBuffer & desc, UniformBuffer * resource)
{
	//UNIMPLEMENTED
}
void RendererBase::Delete(UniformBuffer * resource)
{
	//UNIMPLEMENTED
}
void RendererBase::BindUniformBuffer(UniformBuffer * resource, ShaderProgram * program)
{
	//UNIMPLEMENTED
}
unsigned char * RendererBase::Map(UniformBuffer * resource)
{
	//UNIMPLEMENTED
	return 0;
}
void RendererBase::Unmap(UniformBuffer * resource)
{
	//UNIMPLEMENTED
}