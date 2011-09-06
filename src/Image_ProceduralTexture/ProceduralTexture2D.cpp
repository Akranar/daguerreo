#include "ProceduralTexture2D.h"
#include <cmath>

void ProceduralTexture2D::SetOptions(unsigned int in_width, unsigned int in_height, bool has_alpha, bool _reverse_rgb_endian)
{
	width = in_width;
	height = in_height;
	alpha = has_alpha;
	reverse_rgb_endian = _reverse_rgb_endian;
}

unsigned char * ProceduralTexture2D::AccessTexel(void * data, int x, int y)
{
	int texel_index = y*width + x;
	int byte_index = texel_index * (alpha ? 4 : 3);
	return ((unsigned char *) data)+ byte_index;
}

unsigned char * ProceduralTexture2D::AccessTexel(void * data, int x, int y, unsigned int bytes_per_texel)
{
	int texel_index = y*width + x;
	int byte_index = texel_index * bytes_per_texel;
	return ((unsigned char *) data)+ byte_index;
}

unsigned char ProceduralTexture2D::FloatToByte(float value)
{
	//float clamped = std::max<float>(0, std::min<float>(value, 1.0f));
	unsigned int rtn = (unsigned int) (value * 255);
	return rtn;
}

unsigned int ProceduralTexture2D::GetByteCount()
{
	 return width * height * (alpha ? 4 : 3);
}

void ProceduralTexture2D::GetGradient(void * data, const Eigen::Vector4f corners[4])
{
	for (unsigned int y = 0; y < height; ++y)
	{
		float vertical_factor = float(y)/float(height);
		Eigen::Vector4f left_interp = corners[0] * (1-vertical_factor) + corners[3] * (vertical_factor);
		Eigen::Vector4f right_interp = corners[1] * (1-vertical_factor) + corners[2] * (vertical_factor);
		for (unsigned int x = 0; x < width; ++x)
		{
			float horizontal_factor = float(x)/float(width);
			Eigen::Vector4f final_interp = left_interp * (1-horizontal_factor) + right_interp * (horizontal_factor);
			unsigned char * texel = AccessTexel(data, x, y);
			if (reverse_rgb_endian)
			{
				texel[0] = FloatToByte(final_interp[2]);
				texel[1] = FloatToByte(final_interp[1]);
				texel[2] = FloatToByte(final_interp[0]);
				if (alpha)
				{
					texel[3] = FloatToByte(final_interp[3]);
				}
			}
			else
			{
				texel[0] = FloatToByte(final_interp[0]);
				texel[1] = FloatToByte(final_interp[1]);
				texel[2] = FloatToByte(final_interp[2]);
				if (alpha)
				{
					texel[3] = FloatToByte(final_interp[3]);
				}
			}
		}
	}

}

void ProceduralTexture2D::GetFlatColor(void * data, const Eigen::Vector4f & color)
{
	unsigned char r = FloatToByte(color[0]);
	unsigned char g = FloatToByte(color[1]);
	unsigned char b = FloatToByte(color[2]);
	unsigned char a = FloatToByte(color[3]);
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			unsigned char * texel = AccessTexel(data, x, y);
			if (reverse_rgb_endian)
			{
				texel[0] = b;
				texel[1] = g;
				texel[2] = r;
				if (alpha)
				{
					texel[3] = a;
				}
			}
			else
			{
				texel[0] = r;
				texel[1] = g;
				texel[2] = b;
				if (alpha)
				{
					texel[3] = a;
				}
			}
		}
	}
}

void ProceduralTexture2D::GetHorizontalStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1)
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

	for (unsigned int y = 0; y < height; ++y)
	{
		bool even_stripe = (((y/stripe_width)%2) == 0);
		for (unsigned int x = 0; x < width; ++x)
		{
			unsigned char * texel = AccessTexel(data, x, y);
			if (reverse_rgb_endian)
			{
				if (even_stripe)
				{
					texel[0] = b0;
					texel[1] = g0;
					texel[2] = r0;
					if (alpha) texel[3] = a0;
				}
				else
				{
					texel[0] = b1;
					texel[1] = g1;
					texel[2] = r1;
					if (alpha) texel[3] = a1;
				}
			}
			else
			{
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
	}
}

void ProceduralTexture2D::GetVerticalStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1)
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
		for (unsigned int y = 0; y < height; ++y)
		{
			unsigned char * texel = AccessTexel(data, x, y);

			if (reverse_rgb_endian)
			{
				if (even_stripe)
				{
					texel[0] = b0;
					texel[1] = g0;
					texel[2] = r0;
					if (alpha) texel[3] = a0;
				}
				else
				{
					texel[0] = b1;
					texel[1] = g1;
					texel[2] = r1;
					if (alpha) texel[3] = a1;
				}
			}
			else
			{
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
	}
}

void ProceduralTexture2D::GetGradientGrayscale(void * data, const float corners[4])
{
	for (unsigned int y = 0; y < height; ++y)
	{
		float vertical_factor = float(y)/float(height);
		float left_interp = corners[0] * (1-vertical_factor) + corners[3] * (vertical_factor);
		float right_interp = corners[1] * (1-vertical_factor) + corners[2] * (vertical_factor);
		for (unsigned int x = 0; x < width; ++x)
		{
			float horizontal_factor = float(x)/float(width);
			float final_interp = left_interp * (1-horizontal_factor) + right_interp * (horizontal_factor);
			unsigned char * texel = AccessTexel(data, x, y, 1);
			*texel = FloatToByte(final_interp);
		}
	}
}