#ifndef _INTERFACE_TEXTURE_3D_H_
#define _INTERFACE_TEXTURE_3D_H_

#include "Interface_Texture.h"

class Texture3D : public Texture
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	void * PRIVATE;
	inline Texture3D() : Texture(TEXTURE_TYPE_3D), PRIVATE(0) {};
};



#endif