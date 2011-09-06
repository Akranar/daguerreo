#ifndef _PNG_WRITER_H_
#define _PNG_WRITER_H_

#include "ExLib_LibPNG.h"

class PNGWriter
{
	png_structp png_ptr;
	png_infop info_ptr;
public:
	PNGWriter();
	~PNGWriter();
	int WriteRGB(char* filename, int width, int height, char * buffer, char* title, bool reverse_height);
};

#endif