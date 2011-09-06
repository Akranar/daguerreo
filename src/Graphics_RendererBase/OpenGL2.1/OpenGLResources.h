#ifndef _OPENGL_RESOURCES_H_
#define _OPENGL_RESOURCES_H_

#include "OpenGLResourceStructs.h"

class OpenGLResources
{
public:
	static void NewVertexArray(oglVertexArray * resource);
	static void DeleteVertexArray(oglVertexArray * resource);
	static void EnableVertexArray(oglVertexArray * resource);

	static void NewVertexBuffer(oglVertexBuffer * resource);
	static void DeleteVertexBuffer(oglVertexBuffer * resource);
	static void EnableVertexBuffer(oglVertexBuffer * resource);
	static void * MapVertexBuffer(oglVertexBuffer * resource, GLuint access);
	static void UnmapVertexBuffer();

	static void NewIndexBuffer(oglIndexBuffer * resource);
	static void DeleteIndexBuffer(oglIndexBuffer * resource);
	static void EnableIndexBuffer(oglIndexBuffer * resource);
	static void * MapIndexBuffer(oglIndexBuffer * resource, GLuint access);
	static void UnmapIndexBuffer();
	static void DrawIndexBuffer(oglIndexBuffer * resource);

	static void NewTransformFeedback(oglTransformFeedback * resource);
	static void DeleteTransformFeedback(oglTransformFeedback * resource);
	static void EnableTransformFeedback(oglTransformFeedback * resource);
	static void DisableTransformFeedback(oglTransformFeedback * resource);

	static void NewGLSLProgram(oglGLSLProgram * resource, const char * vs_file, const char * fs_file);
	static void NewGLSLProgramWithTF(oglGLSLProgram * resource, const char * vs_file, const char * fs_file, char ** varyings);
	static void DeleteGLSLProgram(oglGLSLProgram * resource);
	static void EnableGLSLProgram(oglGLSLProgram * resource);

	static void NewTexture1D(oglTexture1D * resource);
	static void DeleteTexture1D(oglTexture1D * resource);
	static void EnableTexture1D(oglTexture1D * resource, unsigned int unit);
	static void * MapTexture1D(oglTexture1D * resource, GLuint access);
	static void UnmapTexture1D(oglTexture1D * resource);

	static void NewTexture2D(oglTexture2D * resource);
	static void DeleteTexture2D(oglTexture2D * resource);
	static void EnableTexture2D(oglTexture2D * resource, unsigned int unit);
	static void * MapTexture2D(oglTexture2D * resource, GLuint access);
	static void UnmapTexture2D(oglTexture2D * resource);

	static void NewTexture3D(oglTexture3D * resource);
	static void DeleteTexture3D(oglTexture3D * resource);
	static void EnableTexture3D(oglTexture3D * resource, unsigned int unit);
	static void * MapTexture3D(oglTexture3D * resource, GLuint access);
	static void UnmapTexture3D(oglTexture3D * resource);

	static void NewTextureCube(oglTextureCube * resource);
	static void DeleteTextureCube(oglTextureCube * resource);
	static void EnableTextureCube(oglTextureCube * resource, unsigned int unit);
	static void * MapTextureCube(oglTextureCube * resource, unsigned int face, GLuint access);
	static void UnmapTextureCube(oglTextureCube * resource, unsigned int face);

	static void NewFrameBuffer(oglRenderTarget * resource);
	static void DeleteFrameBuffer(oglRenderTarget * resource);
	static void EnableFrameBuffer(oglRenderTarget * resource);
	static void AttachColorTexture(oglRenderTarget * resource, oglTexture2D * texture, unsigned int color_unit);
	static void AttachDepthTexture(oglRenderTarget * resource, oglTexture2D * texture);
};

#endif