#ifndef _DESC_TEXTURE_CUBE_H_
#define _DESC_TEXTURE_CUBE_H_

#include "ExLib_Serialization.h"

class DescTextureCube
{
public:
	unsigned int Width;
	unsigned int Height;
	unsigned int MipLevels;
	unsigned int Format;
	unsigned int Usage;
	unsigned int BindFlags;
	void * InitialData[6];
};


#endif