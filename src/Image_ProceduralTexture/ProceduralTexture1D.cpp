#include "ProceduralTexture1D.h"

unsigned char * ProceduralTexture1D::AccessTexel(void * data, int x)
{
	int byte_index = x * (alpha ? 4 : 3);
	return ((unsigned char *) data)+ byte_index;
}
unsigned char * ProceduralTexture1D::AccessTexel(void * data, int x, unsigned int bytes_per_texel)
{
	int byte_index = x * bytes_per_texel;
	return ((unsigned char *) data)+ byte_index;
}
unsigned char ProceduralTexture1D::FloatToByte(float value)
{
	return (unsigned int) (value * 255);
}

void ProceduralTexture1D::SetOptions(unsigned int in_width, bool has_alpha)
{
	width = in_width;
	alpha = has_alpha;
}
void ProceduralTexture1D::GetFlatColor(void * data, const Eigen::Vector4f & color)
{
	unsigned char r = FloatToByte(color[0]);
	unsigned char g = FloatToByte(color[1]);
	unsigned char b = FloatToByte(color[2]);
	unsigned char a = FloatToByte(color[3]);
	for (unsigned int x = 0; x < width; ++x)
	{
		unsigned char * texel = AccessTexel(data, x);
		texel[0] = r;
		texel[1] = g;
		texel[2] = b;
		if (alpha)
		{
			texel[3] = a;
		}
	}
}
void ProceduralTexture1D::GetGradient(void * data, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1)
{
	for (unsigned int x = 0; x < width; ++x)
	{
		float horizontal_factor = float(x)/float(width);
		Eigen::Vector4f interp = color0 * (1-horizontal_factor) + color1 * (horizontal_factor);
		unsigned char * texel = AccessTexel(data, x);
		texel[0] = FloatToByte(interp[0]);
		texel[1] = FloatToByte(interp[1]);
		texel[2] = FloatToByte(interp[2]);
		if (alpha)
		{
			texel[3] = FloatToByte(interp[3]);
		}
	}
}
void ProceduralTexture1D::GetStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1)
{
	unsigned char r0 = FloatToByte(color0[0]);
	unsigned char g0 = FloatToByte(color0[1]);
	unsigned char b0 = FloatToByte(color0[2]);
	unsigned char a0 = FloatToByte(color0[3]);
	unsigned char r1 = FloatToByte(color1[0]);
	unsigned char g1 = FloatToByte(color1[1]);
	unsigned char b1 = FloatToByte(color1[2]);
	unsigned char a1 = FloatToByte(color1[3]);

	unsigned int stripe_width = width / stripe_sections;

	for (unsigned int x = 0; x < width; ++x)
	{
		bool even_stripe = (((x/stripe_width)%2) == 0);

		unsigned char * texel = AccessTexel(data, x);
		if (even_stripe)
		{
			texel[0] = r0;
			texel[1] = g0;
			texel[2] = b0;
			if (alpha) texel[3] = a0;
		}
		else
		{
			texel[0] = r1;
			texel[1] = g1;
			texel[2] = b1;
			if (alpha) texel[3] = a1;
		}
	}
}
void ProceduralTexture1D::GetGradientGrayscale(void * data, const float color0, const float color1)
{
	for (unsigned int x = 0; x < width; ++x)
	{
		float horizontal_factor = float(x)/float(width);
		float interp = color0 * (1-horizontal_factor) + color1 * (horizontal_factor);
		unsigned char * texel = AccessTexel(data, x, 1);
		*texel = FloatToByte(interp);
	}
}
unsigned int ProceduralTexture1D::GetByteCount()
{
	return width * (alpha ? 4 : 3);
}

unsigned int ProceduralTexture1D::GetByteCount(unsigned int channels)
{
	return width * channels;
}