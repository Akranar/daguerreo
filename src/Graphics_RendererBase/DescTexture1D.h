#ifndef _DESC_TEXTURE_1D_H_
#define _DESC_TEXTURE_1D_H_

#include "ExLib_Serialization.h"

class DescTexture1D
{
public:
	unsigned int Width;
	unsigned int MipLevels;
	unsigned int Format;
	unsigned int Usage;
	unsigned int BindFlags;
	void * InitialData;	
};


#endif