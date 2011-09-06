#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

class DescTexture1D;
class DescTexture2D;
class DescTexture3D;
class DescTextureCube;

#include <Core_FixedArray.h>

class TextureLoader
{
public:
	static void LoadTexture1D(const char * filename, DescTexture1D & texture, FixedArray<unsigned char> & data);
	static void LoadTexture2D(const char * filename, DescTexture2D & texture, FixedArray<unsigned char> & data);
	static void LoadTexture3D(const char * filename, DescTexture3D & texture, FixedArray<unsigned char> & data);
	static void LoadTextureCube(const char * filenames[6], DescTextureCube & texture, FixedArray<unsigned char> & data);
};

#endif
