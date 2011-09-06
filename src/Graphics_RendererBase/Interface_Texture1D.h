#ifndef _INTERFACE_TEXTURE_1D_H_
#define _INTERFACE_TEXTURE_1D_H_

#include "Interface_Texture.h"

class Texture1D : public Texture
{
public:
	unsigned int width;
	void * PRIVATE;
	inline Texture1D() : Texture(TEXTURE_TYPE_1D), PRIVATE(0) {};
};



#endif