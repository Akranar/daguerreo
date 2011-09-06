#include "OpenGLResources.h"
#include <string>
#include <fstream>

#include <iostream>
#include "OpenGLCore.h"

//********************************************
//	Contents:
//		-> Vertex Array Object
//		-> Vertex Buffer Object
//		-> Index Buffer Object
//		-> Transform Feedback
//		-> GLSL Program Object
//		-> Texture1D
//		-> Texture2D
//		-> Texture3D
//		-> TextureCube
//		-> Frame Buffer Object
//********************************************


//********************************************
//	Vertex Array Object
//********************************************

void OpenGLResources::NewVertexArray(oglVertexArray * resource)
{
	glGenVertexArrays(1, &resource->vao);
}
void OpenGLResources::DeleteVertexArray(oglVertexArray * resource)
{
	glDeleteVertexArrays(1, &resource->vao);
}
void OpenGLResources::EnableVertexArray(oglVertexArray * resource)
{
	if (resource)
	{
		glBindVertexArray(resource->vao);
	}
	else
	{
		glBindVertexArray(0);
	}
}

//********************************************
//	Vertex Buffer Object
//********************************************

void OpenGLResources::NewVertexBuffer(oglVertexBuffer * resource)
{
	//Generate buffer
	glGenBuffers(1, &resource->buffer);

	//Bind buffer.
	glBindBuffer(GL_ARRAY_BUFFER, resource->buffer);

	//Set buffer size and usage.
	glBufferData(GL_ARRAY_BUFFER, resource->byte_count, 0, resource->buffer_usage);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OpenGLResources::DeleteVertexBuffer(oglVertexBuffer * resource)
{
	glDeleteBuffers(1, &resource->buffer);
}
void OpenGLResources::EnableVertexBuffer(oglVertexBuffer * resource)
{
	if (resource)
		glBindBuffer(GL_ARRAY_BUFFER, resource->buffer);
	else
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void * OpenGLResources::MapVertexBuffer(oglVertexBuffer * resource, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_ARRAY_BUFFER, resource->buffer);
	return glMapBuffer(GL_ARRAY_BUFFER, access);
}
void OpenGLResources::UnmapVertexBuffer()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	//Unbind buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//********************************************
//	Index Buffer Object
//********************************************

void OpenGLResources::NewIndexBuffer(oglIndexBuffer * resource)
{
	//Generate buffer
	glGenBuffers(1, &resource->buffer);

	//Bind buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->buffer);

	//Set buffer size and usage.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, resource->byte_count, 0, resource->buffer_usage);
}
void OpenGLResources::DeleteIndexBuffer(oglIndexBuffer * resource)
{
	glDeleteBuffers(1, &resource->buffer);
}
void OpenGLResources::EnableIndexBuffer(oglIndexBuffer * resource)
{
	if (resource)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->buffer);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void * OpenGLResources::MapIndexBuffer(oglIndexBuffer * resource, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->buffer);
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, access);
}
void OpenGLResources::UnmapIndexBuffer()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	//Unbind buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void OpenGLResources::DrawIndexBuffer(oglIndexBuffer * resource)
{
	glDrawElements(resource->primitive_type, resource->index_count, GL_UNSIGNED_INT, 0);
}

//********************************************
//	Transform Feedback
//********************************************

//#define _QUERY_TRANSFORM_FEEDBACK_

void OpenGLResources::NewTransformFeedback(oglTransformFeedback * resource)
{
	//Generate buffer
	glGenBuffers(1, &resource->buffer);

	//Bind buffer.
	glBindBuffer(GL_ARRAY_BUFFER, resource->buffer);

	//Set buffer size and usage.
	glBufferData(GL_ARRAY_BUFFER, resource->byte_count, 0, resource->buffer_usage);

	//Gen query object
	glGenQueries(1, &resource->query);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OpenGLResources::DeleteTransformFeedback(oglTransformFeedback * resource)
{
	glDeleteBuffers(1, &resource->buffer);
	glDeleteQueries(1, &resource->query);
}
void OpenGLResources::EnableTransformFeedback(oglTransformFeedback * resource)
{
	if (resource)
	{
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, resource->buffer);

#ifdef _QUERY_TRANSFORM_FEEDBACK_
		glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, resource->query);
#endif

		glBeginTransformFeedback(resource->primitive_type);
	}
	else
	{
		glEndTransformFeedback();

#ifdef _QUERY_TRANSFORM_FEEDBACK_
		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
#endif

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	}
}

void OpenGLResources::DisableTransformFeedback(oglTransformFeedback * resource)
{
	glEndTransformFeedback();

#ifdef _QUERY_TRANSFORM_FEEDBACK_
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
#endif

	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

#ifdef _QUERY_TRANSFORM_FEEDBACK_
	GLuint primitives_written = 0;
	glGetQueryObjectuiv(resource->query, GL_QUERY_RESULT, &primitives_written);
#endif
}

//********************************************
//	GLSL Program Object
//********************************************

void OpenGLResources::NewGLSLProgram(oglGLSLProgram * resource, const char * vs_file, const char * fs_file)
{
	std::string vertex_shader_string;
	std::string fragment_shader_string;

	std::fstream stream;
	std::string line;

	stream.open(vs_file, std::ios_base::in);
	while (!stream.eof())
	{
		std::getline(stream, line);
		vertex_shader_string.append(line);
		vertex_shader_string.push_back('\n');
	}
	stream.close();

	stream.clear();

	stream.open(fs_file, std::ios_base::in);
	while (!stream.eof())
	{
		std::getline(stream, line);
		fragment_shader_string.append(line);
		fragment_shader_string.push_back('\n');
	}
	stream.close();

	const char * vertex_shader_text = vertex_shader_string.c_str();
	const GLint vertex_shader_length = vertex_shader_string.length();
	const char * fragment_shader_text = fragment_shader_string.c_str();
	const GLint fragment_shader_length = fragment_shader_string.length();

	//Create OpenGL GLSL Vertex Shader.
	resource->vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(resource->vertex, 1, &vertex_shader_text, &vertex_shader_length);
	glCompileShader(resource->vertex);

	//Create OpenGL GLSL Fragment Shader.
	resource->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(resource->fragment, 1, &fragment_shader_text, &fragment_shader_length);
	glCompileShader(resource->fragment);

	//Create Combined Program.
	resource->combined = glCreateProgram();
	glAttachShader(resource->combined, resource->vertex);
	glAttachShader(resource->combined, resource->fragment);
	glLinkProgram(resource->combined);

	//glUseProgram(resource->combined);

	char log[10000];
	GLsizei length;

	glGetProgramInfoLog(resource->combined, 10000, &length, log);
	if (length > 0)
		std::cout << log << std::endl;

}


void OpenGLResources::NewGLSLProgramWithTF(oglGLSLProgram * resource, const char * vs_file, const char * fs_file, char ** varyings)
{
	std::string vertex_shader_string;
	std::string fragment_shader_string;

	std::fstream stream;
	std::string line;

	stream.open(vs_file, std::ios_base::in);
	while (!stream.eof())
	{
		std::getline(stream, line);
		vertex_shader_string.append(line);
		vertex_shader_string.push_back('\n');
	}
	stream.close();

	stream.clear();

	stream.open(fs_file, std::ios_base::in);
	while (!stream.eof())
	{
		std::getline(stream, line);
		fragment_shader_string.append(line);
		fragment_shader_string.push_back('\n');
	}
	stream.close();

	const char * vertex_shader_text = vertex_shader_string.c_str();
	const GLint vertex_shader_length = vertex_shader_string.length();
	const char * fragment_shader_text = fragment_shader_string.c_str();
	const GLint fragment_shader_length = fragment_shader_string.length();

	//Create OpenGL GLSL Vertex Shader.
	resource->vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(resource->vertex, 1, &vertex_shader_text, &vertex_shader_length);
	glCompileShader(resource->vertex);

	//Create OpenGL GLSL Fragment Shader.
	resource->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(resource->fragment, 1, &fragment_shader_text, &fragment_shader_length);
	glCompileShader(resource->fragment);

	//Create Combined Program.
	resource->combined = glCreateProgram();
	glAttachShader(resource->combined, resource->vertex);
	glAttachShader(resource->combined, resource->fragment);


	//Transform Feedback
	char * Strings[] = {"gl_Position", "gl_TexCoord[0]"};
	glTransformFeedbackVaryings(resource->combined, 2, (const GLchar **) Strings, GL_INTERLEAVED_ATTRIBS);
	//char * Strings[] = {"gl_Position"};
	//glTransformFeedbackVaryings(resource->combined, 1, (const GLchar **) Strings, GL_SEPARATE_ATTRIBS);

	glLinkProgram(resource->combined);

	/*
	GLint position_location = GetVaryingLocationNV(resource->combined, "_position1");
	GLint texcoord_location = GetVaryingLocationNV(resource->combined, "_texcoord01");
	GLint locations[] = {position_location, texcoord_location};
	glTransformFeedbackVaryingsNV(resource->combined, 2, locations, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(resource->combined);
	*/

	//glUseProgram(resource->combined);

	char log[10000];
	GLsizei length;

	glGetProgramInfoLog(resource->combined, 10000, &length, log);
	if (length > 0)
		std::cout << log << std::endl;

}

void OpenGLResources::DeleteGLSLProgram(oglGLSLProgram * resource)
{
	glDetachShader(resource->combined, resource->fragment);
	glDetachShader(resource->combined, resource->vertex);
	glDeleteShader(resource->vertex);
	glDeleteShader(resource->fragment);
	glDeleteProgram(resource->combined);
}
void OpenGLResources::EnableGLSLProgram(oglGLSLProgram * resource)
{
	if (resource)
	{
		glUseProgram(resource->combined);
		//std::cout << (glIsProgram(resource->combined) ? true : false) << std::endl;
		OpenGLCore::PrintAllErrors();
	}
	else
	{
		glUseProgram(0);
	}
}

//********************************************
//	Texture 1D
//********************************************

void OpenGLResources::NewTexture1D(oglTexture1D * resource)
{
	//Set up buffer.
	glGenBuffers(1, &resource->buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, resource->byte_count, 0, resource->buffer_usage);

	//Set up texture.
	glGenTextures(1, &resource->texture);
	glBindTexture(GL_TEXTURE_1D, resource->texture);
	glTexImage1D(GL_TEXTURE_1D, 0, resource->internal_format, resource->width, 0, resource->format, resource->type, 0);

	//Initialize Sampler State
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
}
void OpenGLResources::DeleteTexture1D(oglTexture1D * resource)
{
	glDeleteBuffers(1, &resource->buffer);
	glDeleteTextures(1, &resource->texture);
}
void OpenGLResources::EnableTexture1D(oglTexture1D * resource, unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_1D, resource->texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_1D, 0);
	}
}
void * OpenGLResources::MapTexture1D(oglTexture1D * resource, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, access);
}
void OpenGLResources::UnmapTexture1D(oglTexture1D * resource)
{
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

	glBindTexture(GL_TEXTURE_1D, resource->texture);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, resource->width, resource->format, resource->type, 0);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
}

//********************************************
//	Texture 2D
//********************************************

void OpenGLResources::NewTexture2D(oglTexture2D * resource)
{
	//Set up buffer.
	glGenBuffers(1, &resource->buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, resource->byte_count, 0, resource->buffer_usage);

	//Set up texture.
	glGenTextures(1, &resource->texture);
	glBindTexture(GL_TEXTURE_2D, resource->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, resource->internal_format, resource->width, resource->height, 0, resource->format, resource->type, 0);

	//Initialize Sampler State
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void OpenGLResources::DeleteTexture2D(oglTexture2D * resource)
{
	glDeleteBuffers(1, &resource->buffer);
	glDeleteTextures(1, &resource->texture);
}
void OpenGLResources::EnableTexture2D(oglTexture2D * resource, unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_2D, resource->texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void * OpenGLResources::MapTexture2D(oglTexture2D * resource, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, access);
}
void OpenGLResources::UnmapTexture2D(oglTexture2D * resource)
{
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

	glBindTexture(GL_TEXTURE_2D, resource->texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, resource->width, resource->height, resource->format, resource->type, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//********************************************
//	Texture 3D
//********************************************

void OpenGLResources::NewTexture3D(oglTexture3D * resource)
{
	//Set up buffer.
	glGenBuffers(1, &resource->buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, resource->byte_count, 0, resource->buffer_usage);

	//Set up texture.
	glGenTextures(1, &resource->texture);
	glBindTexture(GL_TEXTURE_3D, resource->texture);
	glTexImage3D(GL_TEXTURE_3D, 0, resource->internal_format, resource->width, resource->height, resource->thickness, 0, resource->format, resource->type, 0);

	//Initialize Sampler State
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
}
void OpenGLResources::DeleteTexture3D(oglTexture3D * resource)
{
	glDeleteBuffers(1, &resource->buffer);
	glDeleteTextures(1, &resource->texture);
}
void OpenGLResources::EnableTexture3D(oglTexture3D * resource, unsigned int unit)
{
	//glActiveTexture(GL_TEXTURE0 + unit);
	//glBindTexture(GL_TEXTURE_3D, resource->texture);

	glActiveTexture(GL_TEXTURE0 + unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_3D, resource->texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_3D, 0);
	}
}
void * OpenGLResources::MapTexture3D(oglTexture3D * resource, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer);
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, access);
}
void OpenGLResources::UnmapTexture3D(oglTexture3D * resource)
{
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);
	//Unbind buffer.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}

//********************************************
//	Texture Cube
//********************************************

void OpenGLResources::NewTextureCube(oglTextureCube * resource)
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		glGenBuffers(1, &resource->buffer[i]);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer[i]);
		glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, resource->byte_count, 0, resource->buffer_usage);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}

	glGenTextures(1, &resource->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, resource->texture);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, resource->internal_format, resource->width, resource->height, 0, resource->format, resource->type, 0);
	}

	//Initialize Sampler State
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
void OpenGLResources::DeleteTextureCube(oglTextureCube * resource)
{
	glDeleteBuffers(6, resource->buffer);
	glDeleteTextures(1, &resource->texture);
}
void OpenGLResources::EnableTextureCube(oglTextureCube * resource, unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, resource->texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
void * OpenGLResources::MapTextureCube(oglTextureCube * resource, unsigned int face, GLuint access)
{
	//Bind buffer.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, resource->buffer[face]);
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, access);
}
void OpenGLResources::UnmapTextureCube(oglTextureCube * resource, unsigned int face)
{
	//Unmap buffer.
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

	//TexSubImage the data.
	glBindTexture(GL_TEXTURE_CUBE_MAP, resource->texture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, 0, 0, resource->width, resource->height, resource->format, resource->type, 0);

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//********************************************
//	Frame Buffer Object
//********************************************

void OpenGLResources::NewFrameBuffer(oglRenderTarget * resource)
{
	glGenFramebuffersEXT(1, &resource->fbo_handle);
}
void OpenGLResources::DeleteFrameBuffer(oglRenderTarget * resource)
{
	glDeleteFramebuffersEXT(1, &resource->fbo_handle);
}

void OpenGLResources::AttachColorTexture(oglRenderTarget * resource, oglTexture2D * texture, unsigned int color_unit)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, resource->fbo_handle);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + color_unit, GL_TEXTURE_2D, 
		texture->texture, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Framebuffer Initialization Success" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Attachment" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "ERROR: Framebuffer Missing Attachment" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "ERROR: Framebuffer Different Dimensions" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "ERROR: Framebuffer Different Internal Formats" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Draw Buffer" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Read Buffer" << std::endl;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "ERROR: Framebuffer Unsupported Extension" << std::endl;
			break;

		default:
			std::cout << "ERROR: Framebuffer Unknown Error" << std::endl;
			break;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void OpenGLResources::AttachDepthTexture(oglRenderTarget * resource, oglTexture2D * texture)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, resource->fbo_handle);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, texture->texture, 0);

	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, texture->texture, 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			//std::cout << "Framebuffer Initialization Success" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Attachment" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "ERROR: Framebuffer Missing Attachment" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "ERROR: Framebuffer Different Dimensions" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "ERROR: Framebuffer Different Internal Formats" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Draw Buffer" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "ERROR: Framebuffer Incomplete Read Buffer" << std::endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "ERROR: Framebuffer Unsupported Extension" << std::endl;
			break;
		default:
			std::cout << "ERROR: Framebuffer Unknown Error" << std::endl;
			break;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLResources::EnableFrameBuffer(oglRenderTarget * resource)
{
	if (resource)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, resource->fbo_handle);
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}