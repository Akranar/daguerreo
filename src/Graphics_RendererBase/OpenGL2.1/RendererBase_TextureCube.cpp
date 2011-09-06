#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_TextureCube.h"
#include "../Interface_ShaderProgram.h"
#include "../DescTextureCube.h"

#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"



//*************************************
//	TextureCube
//*************************************

void RendererBase::CreateResource(const DescTextureCube & desc, TextureCube * resource)
{
	GLuint texture;
	GLuint internal_format = GL_RGBA8;
	GLuint format = GL_RGBA;
	GLuint type = GL_UNSIGNED_BYTE;

	resource->width = desc.Width;
	resource->height = desc.Height;
	resource->miplevels = desc.MipLevels;

	//Set up texture.
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		if (desc.InitialData[i])
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internal_format, desc.Width, desc.Height, 0, format, type, desc.InitialData[i]);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internal_format, desc.Width, desc.Height, 0, format, type, 0);
		}
	}

	if (resource->miplevels == 0)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	resource->PRIVATE = (void *) texture;
}
void RendererBase::Delete(TextureCube * resource)
{
	GLuint texture = (GLuint) resource->PRIVATE;
	glDeleteTextures(1, &texture);
}
void RendererBase::Bind(ShaderProgram * program, unsigned int unit, TextureCube * resource)
{
	ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
	unsigned int converted_unit = program_pimpl->sampler_units[unit];
	glActiveTexture(GL_TEXTURE0 + converted_unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint) resource->PRIVATE);
	}
	else
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
void * RendererBase::Map(TextureCube * resource, MapAccess access, unsigned int face)
{
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pimpl->GetAvailablePBO());
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, MapAccessMap[access]);
}
void RendererBase::Unmap(TextureCube * resource, unsigned int face)
{
	GLuint format = GL_RGBA;
	GLuint type = GL_UNSIGNED_BYTE;

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pimpl->pbo_pool[0]);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint) resource->PRIVATE);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, 0, 0, resource->width, resource->height, format, type, 0);

	if (resource->miplevels == 0)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}