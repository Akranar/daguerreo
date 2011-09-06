#ifndef _INTERFACE_TEXTURE_H_
#define _INTERFACE_TEXTURE_H_

#include "Enumerants.h"
class Texture1D;
class Texture2D;
class Texture3D;
class TextureCube;

class Texture
{
protected:
	unsigned int texture_type;
public:
	Texture(unsigned int type) : texture_type(type) {};
	virtual ~Texture() {};
	inline unsigned int GetType() const;
	inline Texture1D * Cast1D();
	inline Texture2D * Cast2D();
	inline Texture3D * Cast3D();
	inline TextureCube * CastCube();
};

inline unsigned int Texture::GetType() const
{
	return texture_type;
}
inline Texture1D * Texture::Cast1D()
{
	return (Texture1D *) this;
}

inline Texture2D * Texture::Cast2D()
{
	return (Texture2D *) this;
}

inline Texture3D * Texture::Cast3D()
{
	return (Texture3D *) this;
}

inline TextureCube * Texture::CastCube()
{
	return (TextureCube *) this;
}

#endif