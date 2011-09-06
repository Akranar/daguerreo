#ifndef _INTERFACE_TEXTURE_CUBE_H_
#define _INTERFACE_TEXTURE_CUBE_H_

#include "Interface_Texture.h"

class TextureCube : public Texture
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int miplevels;
	void * PRIVATE;
	inline TextureCube() : Texture(TEXTURE_TYPE_CUBE), PRIVATE(0) {};
};

#endif