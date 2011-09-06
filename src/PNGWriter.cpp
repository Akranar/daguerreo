#include "PNGWriter.h"
#include "ExLib_LibPNG.h"

PNGWriter::PNGWriter()
{
	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
}
PNGWriter::~PNGWriter()
{
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

//FIX THIS LATER. GCC doesn't like finalise.
int PNGWriter::WriteRGB(char* filename, int width, int height, char * buffer, char* title, bool reverse_height)
{
	/*
	int code = 0;
	FILE *fp;
	
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);


	row = (png_bytep) buffer;
	unsigned int stride = 3 * width * sizeof(png_byte);
	for (unsigned int y = 0; y < height; ++y)
	{
		if (reverse_height)
		{
			row = (png_bytep) buffer + ((height - y - 1) * stride);
		}	
		else
		{
			row = (png_bytep) buffer + (y * stride);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	//if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	//if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

	return code;
	*/
	return 0;
}