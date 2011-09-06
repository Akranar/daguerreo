#ifndef _PROCEDURAL_TEXTURE_2D_H_
#define _PROCEDURAL_TEXTURE_2D_H_

#include "ExLib_Eigen.h"

class ProceduralTexture2D
{
	bool alpha;
	unsigned int width, height;
	unsigned char * AccessTexel(void * data, int x, int y);
	unsigned char * AccessTexel(void * data, int x, int y, unsigned int bytes_per_texel);
	unsigned char FloatToByte(float value);
	bool reverse_rgb_endian;
public:
	void SetOptions(unsigned int width, unsigned int height, bool has_alpha, bool reverse_rgb_endian);
	void GetGradient(void * data, const Eigen::Vector4f corners[4]);
	void GetFlatColor(void * data, const Eigen::Vector4f & color);
	void GetHorizontalStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1);
	void GetVerticalStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1);
	void GetGradientGrayscale(void * data, const float corners[4]);
	unsigned int GetByteCount();
};

#endif