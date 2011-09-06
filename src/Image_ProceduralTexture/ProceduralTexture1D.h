#ifndef _PROCEDURAL_TEXTURE_1D_H_
#define _PROCEDURAL_TEXTURE_1D_H_

#include "ExLib_Eigen.h"

class ProceduralTexture1D
{
	bool alpha;
	unsigned int width;
	unsigned char * AccessTexel(void * data, int x);
	unsigned char * AccessTexel(void * data, int x, unsigned int bytes_per_texel);
	unsigned char FloatToByte(float value);
public:
	void SetOptions(unsigned int width, bool has_alpha);
	void GetFlatColor(void * data, const Eigen::Vector4f & color);
	void GetGradient(void * data, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1);
	void GetStripes(void * data, unsigned int stripe_sections, const Eigen::Vector4f & color0, const Eigen::Vector4f & color1);
	void GetGradientGrayscale(void * data, const float color0, const float color1);
	unsigned int GetByteCount();
	unsigned int GetByteCount(unsigned int channels);
};

#endif