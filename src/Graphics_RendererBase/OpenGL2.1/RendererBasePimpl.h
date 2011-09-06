#ifndef _RENDERER_BASE_PIMPL_H_
#define _RENDERER_BASE_PIMPL_H_

#include "ExLib_API_OpenGL.h"
#include <vector>
#include <map>
#include "OglInputLayout.h"


class ShaderProgram;
class VertexBuffer;
class IndexBuffer;

class RendererBasePimpl
{
public:
	std::vector<GLuint> pbo_pool;
	GLuint GetAvailablePBO();
	std::map<unsigned int, std::map<unsigned int, OglInputLayout> > input_layouts;
	OglInputLayout * bound_layout;
	VertexBuffer * bound_vertex_buffer;
	IndexBuffer * bound_index_buffer;
	ShaderProgram * bound_shader_program;
	RendererBasePimpl() : bound_layout(0), bound_vertex_buffer(0), bound_index_buffer(0), bound_shader_program(0) {};
	void InputLayoutHelper_AppendZeroIfLastAlpha(std::string & name);
	bool InputLayoutHelper_NameToUsage(const std::string & name, unsigned int & rtn_usage_length, unsigned int & rtn_usage_index);
	void GetTypeInfo(GLuint gl_type, unsigned char & type, unsigned char & rows, unsigned char & columns, bool & sampler);
	void InitDomainUniformAndSamplers(ShaderProgram * resource, unsigned int domain, GLuint program);
	void InitDomainUniformBufferDecl(ShaderProgram * resource, unsigned int domain, GLuint program);
};


#endif