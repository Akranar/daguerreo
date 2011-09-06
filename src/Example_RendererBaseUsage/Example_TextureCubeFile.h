#ifndef _EXAMPLE_TEXTURE_CUBE_FILE_H_
#define _EXAMPLE_TEXTURE_CUBE_FILE_H_

#include "ExLib_RendererBase.h"
#include "ExLib_TextureLoader.h"

class Example_TextureCubeFile
{
	TextureCube texture_cube;
	ShaderProgram * shader_program;
	RendererBase * renderer;
	unsigned int unit;
public:
	void Init(RendererBase * _renderer, ShaderProgram * _shader_program, unsigned int _unit, const char * filenames[6])
	{
		shader_program = _shader_program;
		renderer = _renderer;
		unit = _unit;


		DescTextureCube texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		
		FixedArray<unsigned char> texdata;
		TextureLoader::LoadTextureCube(filenames, texdesc, texdata);
		renderer->CreateResource(texdesc, &texture_cube);

	}

	void Deinit()
	{
		renderer->Delete(&texture_cube);
	}

	void PreRender()
	{
		renderer->Bind(shader_program, unit, &texture_cube);
	}
};
#endif