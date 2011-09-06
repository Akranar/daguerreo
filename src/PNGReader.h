#ifndef _PNG_READER_H_
#define _PNG_READER_H_

#include "ExLib_LibPNG.h"

class PNGReadStream;

class PNGReader
{
	png_structp png_ptr;
	png_info * pinfo;
public:
	PNGReader();
	~PNGReader();
	void SetReadStream(PNGReadStream * stream);
	void ReadPNGHeader();
	unsigned int GetWidth();
	unsigned int GetHeight();
	unsigned int GetBitsPerChannel();
	unsigned int GetChannelsCount();
	unsigned int GetColorType();
	unsigned int GetStride();
	unsigned int GetByteCount();
	void SetRGBA8();
	void SetGrayToRGB8();
	void SetRGB8ToGray();
	void SetRGB8ToNormalizedGray();
	void Set16To8();
	void ReadImage(char * dest, char ** row_ptr_buffer, bool reverse_height);
	static bool IsPNG(const char * bytes, unsigned int num_bytes_to_check);
	static void PNGStreamReadFn(png_structp png_ptr, png_bytep out_bytes, png_size_t bytes_to_read);
};

#endif