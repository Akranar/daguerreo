#ifndef _DESC_TEXTURE_3D_H_
#define _DESC_TEXTURE_3D_H_

#include "ExLib_Serialization.h"

class DescTexture3D
{
public:
	unsigned int Width;
	unsigned int Height;
	unsigned int Depth;
	unsigned int MipLevels;
	unsigned int Format;
	unsigned int Usage;
	unsigned int BindFlags;
	void * InitialData;
};


#endif