#ifndef _EXAMPLE_TEXTURE_2D_H_
#define _EXAMPLE_TEXTURE_2D_H_

#include "ExLib_RendererBase.h"
#include "ExLib_ProceduralTexture.h"

class Example_Texture2D
{
	Texture2D texture_2d;
	ShaderProgram * shader_program;
	RendererBase * renderer;
	unsigned int domain;
	unsigned int unit;
public:
	void Init(RendererBase * _renderer, ShaderProgram * _shader_program, unsigned int _domain, unsigned int _unit, unsigned int option)
	{
		shader_program = _shader_program;
		renderer = _renderer;
		domain = _domain;
		unit = _unit;

		ProceduralTexture2D proc2d;
		Eigen::Vector4f colors[4];
		float intensity = 1.0f;
		int width = 256;
		colors[0] = Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f) * intensity; //Red
		colors[1] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f) * intensity; //Green
		colors[2] = Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f) * intensity; //Blue
		colors[3] = Eigen::Vector4f(0.7f, 0.7f, 0.0f, 1.0f) * intensity; //Yellow
		proc2d.SetOptions(width, width, true, false);


		DescTexture2D texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.Format = PF_32_R8G8B8A8;
		texdesc.Width = width;
		texdesc.Height = width;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		
		bool use_map = false;
		if (use_map)
		{
			texdesc.InitialData = 0;
			renderer->CreateResource(texdesc, &texture_2d);
			void * texdata = renderer->Map(&texture_2d, MAP_WRITE_ONLY);
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
			renderer->Unmap(&texture_2d);
		}
		else
		{
			unsigned int byte_count = proc2d.GetByteCount();
			unsigned char * initial_data = new unsigned char[byte_count];
			texdesc.InitialData = initial_data;
			switch(option)
			{
				case 0:
					proc2d.GetGradient(initial_data, colors);
					break;
				case 1:
					proc2d.GetHorizontalStripes(initial_data, 50, colors[0], colors[2]);
					break;
				case 2:
					proc2d.GetVerticalStripes(initial_data, 50, colors[0], colors[2]);
					break;
			}
			renderer->CreateResource(texdesc, &texture_2d);
			delete [] initial_data;
		}
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