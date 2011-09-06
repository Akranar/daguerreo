#include "Mapping.h"
#include "../RendererBase.h"
#include "../Interface_Texture2D.h"
#include "../Interface_ShaderProgram.h"
#include "../DescTexture2D.h"

#include "RendererBasePimpl.h"
#include "ShaderProgramPimpl.h"


//*************************************
//	Texture2D
//*************************************

void RendererBase::CreateResource(const DescTexture2D & desc, Texture2D * resource)
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
	glBindTexture(GL_TEXTURE_2D, texture);
	if (desc.InitialData)
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, desc.Width, desc.Height, 0, format, type, desc.InitialData);

		if (resource->miplevels == 0)
		{
			glGenerateMipmap(GL_TEXTURE_2D);//
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, desc.Width, desc.Height, 0, format, type, 0);
	}

	//Initialize Sampler State
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Unbind buffer and texture.
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	resource->PRIVATE = (void *) texture;
}
void RendererBase::Delete(Texture2D * resource)
{
	GLuint texture = (GLuint) resource->PRIVATE;
	glDeleteTextures(1, &texture);
}
void RendererBase::Bind(ShaderProgram * program, unsigned int unit, Texture2D * resource)
{
	ShaderProgramPimpl * program_pimpl = (ShaderProgramPimpl *) program->PRIVATE;
	unsigned int converted_unit = program_pimpl->sampler_units[unit];
	glActiveTexture(GL_TEXTURE0 + converted_unit);
	if (resource)
	{
		glBindTexture(GL_TEXTURE_2D, (GLuint) resource->PRIVATE);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void * RendererBase::Map(Texture2D * resource, MapAccess access)
{
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pimpl->GetAvailablePBO());
	return glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, MapAccessMap[access]);
}
void RendererBase::Unmap(Texture2D * resource)
{
	GLuint format = GL_RGBA;
	GLuint type = GL_UNSIGNED_BYTE;

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pimpl->pbo_pool[0]);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);

	glBindTexture(GL_TEXTURE_2D, (GLuint) resource->PRIVATE);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, resource->width, resource->height, format, type, 0);

	if (resource->miplevels == 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	//Unbind buffer and texture.
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
