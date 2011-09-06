#ifndef _EXAMPLE_TEXTURE_2D_FILE_H_
#define _EXAMPLE_TEXTURE_2D_FILE_H_

#include "ExLib_RendererBase.h"
#include "ExLib_TextureLoader.h"

class Example_Texture2DFile
{
public:
	Texture2D texture_2d;
	ShaderProgram * shader_program;
	RendererBase * renderer;
	unsigned int unit;

	void Init(RendererBase * _renderer, ShaderProgram * _shader_program, unsigned int _unit, const char * filename)
	{
		shader_program = _shader_program;
		renderer = _renderer;
		unit = _unit;

		
		DescTexture2D texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		FixedArray<unsigned char> texdata;

		TextureLoader::LoadTexture2D(filename, texdesc, texdata);
		renderer->CreateResource(texdesc, &texture_2d);
	}

	void Deinit()
	{
		renderer->Delete(&texture_2d);
	}

	void PreRender()
	{
		renderer->Bind(shader_program, unit, &texture_2d);
	}
};
#endif
