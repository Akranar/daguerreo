#ifndef _INTERFACE_TEXTURE_2D_H_
#define _INTERFACE_TEXTURE_2D_H_

#include "Interface_Texture.h"

class Texture2D : public Texture
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int miplevels;
	void * PRIVATE;
	inline Texture2D() : Texture(TEXTURE_TYPE_2D), PRIVATE(0) {};
};



#endif