#include "../RendererBase.h"
#include "../ResourceDescs.h"
#include "../IncludeAll_Interface.h"

#include "RendererBasePimpl.h"
#include "Mapping.h"

#include <iostream>
#include <map>
#include <vector>


//*************************************
//	OpenGL 2.1 Implementation BEGIN
//*************************************

#include "ExLib_API_OpenGL.h"

void RendererBase::ClearAllBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void RendererBase::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void RendererBase::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}
void RendererBase::ClearStencilBuffer()
{
	glClear( GL_STENCIL_BUFFER_BIT);
}
void RendererBase::ClearDepthStencilBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererBase::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}
void RendererBase::ClearDepth(double depth)
{
	glClearDepth(depth);
}
void RendererBase::ClearStencil(int stencil)
{
	glClearStencil(stencil);
}

RendererBase::RendererBase()
: 
pimpl(0)
{
	PlatformIndependentInit();

	GLint max_vertex_uniforms;
	GLint max_fragment_uniforms;
	GLint max_geometry_uniforms;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniforms);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniforms);
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &max_geometry_uniforms);

	std::cout << "Max Vertex Uniforms: " << max_vertex_uniforms << std::endl;
	std::cout << "Max Fragment Uniforms: " << max_fragment_uniforms << std::endl;
	std::cout << "Max Geometry Uniforms: " << max_geometry_uniforms << std::endl;

}

RendererBase::~RendererBase()
{
	DestroyHashers();
	for (unsigned int i = 0; i < pimpl->pbo_pool.size(); ++i)
	{
		glDeleteBuffers(1, &pimpl->pbo_pool[i]);
	}
	if (pimpl)
	{
		DestroyState(&DefaultBlend);
		DestroyState(&DefaultDepthStencil);
		DestroyState(&DefaultRasterizer);
		DestroyState(&DefaultSampler);
		delete pimpl;
	}
}

void RendererBase::Initialize(WindowEvents * window)
{	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Initialization Failed." << std::endl;
		throw 1;
	}

	pimpl = new RendererBasePimpl();
	CreateDefaultStates(&DefaultBlend, &DefaultDepthStencil, &DefaultRasterizer, &DefaultSampler);
	SetState(&DefaultBlend);
	SetState(&DefaultDepthStencil);
	SetState(&DefaultRasterizer);

	bool uniform_buffers = GLEW_ARB_uniform_buffer_object;
	bool bindables = GLEW_EXT_bindable_uniform;

	unsigned int byte_count = 4096 * 4096 * 4;
	for (unsigned int i = 0; i < 3; ++i)
	{
		//Set up buffer.
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, buffer);
		glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, byte_count, 0, GL_STREAM_DRAW);
		pimpl->pbo_pool.push_back(buffer);
	}
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	
}

void RendererBase::PreRender()
{

}

void RendererBase::Draw()
{
	glDrawElements(MappingPrimitiveType[pimpl->bound_index_buffer->primitive_type], pimpl->bound_index_buffer->byte_count / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

void RendererBase::PostRender()
{
	if (pimpl->bound_layout)
	{
		pimpl->bound_layout->End();
	}
	BindIndexBuffer(0);
	BindVertexBuffer(0);
	BindShaderProgram(0);
	glFlush();
}

void RendererBase::OnViewportUpdate(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}
