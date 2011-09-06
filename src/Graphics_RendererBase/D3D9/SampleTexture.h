#ifndef _SAMPLE_TEXTURE_H_
#define _SAMPLE_TEXTURE_H_

#include "../RendererBase.h"

#include "../ExLib_ProceduralTexture.h"

class SampleTexture
{
	Texture2D texture_2d;
public:
	void Init(RendererBase & renderer)
	{
		DescTexture2D texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
		texdesc.Format = PF_32_R8G8B8A8;
		texdesc.Width = 512;
		texdesc.Height = 512;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		texdesc.InitialData = 0;

		ProceduralTexture2D proc;
		proc.SetOptions(512, 512, true, true);
		unsigned int byte_count = proc.GetByteCount();

		unsigned char * initial_data = new unsigned char[byte_count];
		Eigen::Vector4f colors[4];
		float intensity = 1.0f;
		colors[0] = Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f) * intensity; //Red
		colors[1] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f) * intensity; //Green
		colors[2] = Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f) * intensity; //Blue
		colors[3] = Eigen::Vector4f(0.7f, 0.7f, 0.0f, 1.0f) * intensity; //Yellow
		
		//proc.GetHorizontalStripes(initial_data, 50, colors[2], colors[0]);
		proc.GetGradient(initial_data, colors);
		texdesc.InitialData = initial_data;
		renderer.CreateResource(texdesc, &texture_2d);
		delete initial_data;
	}

	void Deinit(RendererBase & renderer)
	{
		renderer.Delete(&texture_2d);
	}

	void PreRender(RendererBase & renderer)
	{
		renderer.Bind(0, 0, &texture_2d);
	}

	void PostRender(RendererBase & renderer)
	{
		renderer.Bind(0, 0, (Texture2D *) 0);
	}
};

#endif