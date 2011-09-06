#ifndef _EXAMPLE_TEXTURE_CUBE_H_
#define _EXAMPLE_TEXTURE_CUBE_H_

#include "RendererBase.h"
#include "ExLib_ProceduralTexture.h"

class Example_TextureCube
{
	TextureCube texture_cube;
	ShaderProgram * shader_program;
	RendererBase * renderer;
	unsigned int unit;
public:
	void Init(RendererBase * _renderer, ShaderProgram * _shader_program, unsigned int _unit, unsigned int option)
	{
		shader_program = _shader_program;
		renderer = _renderer;
		unit = _unit;

		ProceduralTexture2D proc2d;
		Eigen::Vector4f colors[6];
		float intensity = 1.0f;
		int width = 256;
		colors[0] = Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f) * intensity; //Red
		colors[1] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f) * intensity; //Green
		colors[2] = Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f) * intensity; //Blue
		colors[3] = Eigen::Vector4f(0.9f, 0.9f, 0.0f, 1.0f) * intensity; //Yellow
		colors[4] = Eigen::Vector4f(0.4f, 0.7f, 1.0f, 1.0f) * intensity; //Sky
		colors[5] = Eigen::Vector4f(0.4f, 0.2f, 0.0f, 1.0f) * intensity; //Brown
		proc2d.SetOptions(width, width, true, false);


		DescTextureCube texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.Format = PF_32_R8G8B8A8;
		texdesc.Width = width;
		texdesc.Height = width;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		
		bool use_map = false;
		if (use_map)
		{
			for (unsigned int face = 0; face < 6; ++face)
			{
				texdesc.InitialData[face] = 0;
				renderer->CreateResource(texdesc, &texture_cube);
				void * texdata = renderer->Map(&texture_cube, MAP_WRITE_ONLY, face);
				switch(option)
				{
					case 0:
						proc2d.GetGradient(texdata, colors);
						break;
					case 1:
						proc2d.GetHorizontalStripes(texdata, 50, colors[0], colors[2]);
						break;
					case 2:
						proc2d.GetVerticalStripes(texdata, 50, colors[0], colors[2]);
						break;
				}
				renderer->Unmap(&texture_cube, face);
			}
		}
		else
		{
			unsigned int byte_count = proc2d.GetByteCount();
			unsigned char * initial_data = new unsigned char[byte_count * 6];
			for (unsigned int face = 0; face < 6; ++face)
			{
				texdesc.InitialData[face] = initial_data + (byte_count * face);
			
				proc2d.GetFlatColor(texdesc.InitialData[face], colors[face]);
				/*
				switch(option)
				{
					case 0:
						proc2d.GetGradient(texdesc.InitialData[face], colors);
						break;
					case 1:
						proc2d.GetHorizontalStripes(texdesc.InitialData[face], 50, colors[0], colors[2]);
						break;
					case 2:
						proc2d.GetVerticalStripes(texdesc.InitialData[face], 50, colors[0], colors[2]);
						break;
				}
				*/
			}
			renderer->CreateResource(texdesc, &texture_cube);
			delete [] initial_data;

		}
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