#ifndef _RESOURCE_STRUCTS_H_
#define _RESOURCE_STRUCTS_H_

#include "ExLib_API_OpenGL.h"

class oglTransformFeedback
{
public:
	GLuint buffer;
	GLuint query;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int vertex_count;
	GLuint primitive_type;
	void SetOptions(unsigned int _byte_count, unsigned int _buffer_usage, unsigned int _vertex_count, GLuint _primitive_type)
	{
		byte_count = _byte_count;
		buffer_usage = _buffer_usage;
		vertex_count = _vertex_count;
		primitive_type = _primitive_type;
	}
};

class oglVertexArray
{
public:
	GLuint vao;
};

class oglVertexBuffer
{
public:
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int vertex_count;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_vertex_count)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		vertex_count = in_vertex_count;
	}
};

class oglIndexBuffer
{
public:
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int index_count;
	GLuint primitive_type;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_index_count, GLuint _primitive_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		index_count = in_index_count;
		primitive_type = _primitive_type;
	}
};

class oglTexture1D
{
public:
	GLuint texture;
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int width;
	GLuint format;
	GLuint internal_format;
	GLuint type;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_width, GLuint in_format, GLuint in_internal_format, GLuint in_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		width = in_width;
		format = in_format;
		internal_format = in_internal_format;
		type = in_type;
	}
};

class oglTexture2D
{
public:
	GLuint texture;
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int width;
	unsigned int height;
	GLuint format;
	GLuint internal_format;
	GLuint type;
	
	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_width, unsigned int in_height, GLuint in_format, GLuint in_internal_format, GLuint in_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		width = in_width;
		height = in_height;
		format = in_format;
		internal_format = in_internal_format;
		type = in_type;
	}
	
};

class oglTexture3D
{
public:
	GLuint texture;
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int width;
	unsigned int height;
	unsigned int thickness;
	GLuint format;
	GLuint internal_format;
	GLuint type;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_width, unsigned int in_height, unsigned int in_thickness, GLuint in_format, GLuint in_internal_format, GLuint in_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		width = in_width;
		height = in_height;
		thickness = in_thickness;
		format = in_format;
		internal_format = in_internal_format;
		type = in_type;
	}
};

class oglTextureCube
{
public:
	GLuint texture;
	GLuint buffer[6];
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int width;
	unsigned int height;
	GLuint format;
	GLuint internal_format;
	GLuint type;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_width, unsigned int in_height, GLuint in_format, GLuint in_internal_format, GLuint in_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		width = in_width;
		height = in_height;
		format = in_format;
		internal_format = in_internal_format;
		type = in_type;
	}
};

class oglTextureRect
{
public:
	GLuint texture;
	GLuint buffer;
	//Options
	unsigned int byte_count;
	GLuint buffer_usage;
	unsigned int width;
	unsigned int height;
	GLuint format;
	GLuint internal_format;
	GLuint type;

	void SetOptions(unsigned int in_byte_count, unsigned int in_buffer_usage, unsigned int in_width, GLuint in_format, GLuint in_internal_format, GLuint in_type)
	{
		byte_count = in_byte_count;
		buffer_usage = in_buffer_usage;
		width = in_width;
		format = in_format;
		internal_format = in_internal_format;
		type = in_type;
	}
};

class oglGLSLProgram
{
public:
	GLuint vertex;
	GLuint fragment;
	GLuint combined;
};

class oglRenderTarget
{
public:
	GLuint fbo_handle;
	unsigned int width;
	unsigned int height;

	void SetOptions(unsigned int _width, unsigned int _height)
	{
		width = _width;
		height = _height;
	}
};

#endif