#include "PNGReader.h"
#include "PNGReadStream.h"

PNGReader::PNGReader()
{
	//Create PNG structures;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	pinfo = png_create_info_struct(png_ptr);
	
}

PNGReader::~PNGReader()
{
	png_destroy_read_struct(&png_ptr, &pinfo, (png_infopp) 0);
}

void PNGReader::SetReadStream(PNGReadStream * stream)
{
	//Set custom read function pointer.
	png_set_read_fn(png_ptr, stream, PNGReader::PNGStreamReadFn);
}

void PNGReader::ReadPNGHeader()
{
	png_read_info(png_ptr, pinfo);
}

unsigned int PNGReader::GetWidth()
{
	return png_get_image_width(png_ptr, pinfo);
}

unsigned int PNGReader::GetHeight()
{
	return png_get_image_height(png_ptr, pinfo);
}

unsigned int PNGReader::GetBitsPerChannel()
{
	return png_get_bit_depth(png_ptr, pinfo);
}

unsigned int PNGReader::GetChannelsCount()
{
	return png_get_channels(png_ptr, pinfo);
}

unsigned int PNGReader::GetColorType()
{
	return png_get_color_type(png_ptr, pinfo);
}

unsigned int PNGReader::GetStride()
{
	return GetWidth() * GetBitsPerChannel() * GetChannelsCount() / 8;
}

unsigned int PNGReader::GetByteCount()
{
	return GetWidth() * GetHeight() * GetBitsPerChannel() * GetChannelsCount() / 8;
}

void PNGReader::SetRGBA8()
{
	png_set_strip_16(png_ptr);
	png_set_expand(png_ptr);
}

void PNGReader::SetGrayToRGB8()
{
	png_set_gray_to_rgb(png_ptr);
}

void PNGReader::SetRGB8ToGray()
{
	png_set_rgb_to_gray(png_ptr, 1, 0, 0);
}

void PNGReader::SetRGB8ToNormalizedGray()
{
	//http://www.libpng.org/pub/png/libpng-1.0.3-manual.html
	png_set_rgb_to_gray(png_ptr, 1, 0.211, 0.715);
}

void PNGReader::Set16To8()
{
	png_set_strip_16(png_ptr);
}

void PNGReader::ReadImage(char * dest, char ** row_ptr_buffer, bool reverse_height)
{
	const unsigned int stride = GetWidth() * GetBitsPerChannel() * GetChannelsCount() / 8;

	const unsigned int image_height = GetHeight();
	for (unsigned int i = 0; i < image_height; ++i)
	{
		unsigned int dest_position;
		if (reverse_height)
		{
			dest_position = (image_height - i - 1) * stride;
		}
		else
		{
			dest_position = stride * i;
		}
		row_ptr_buffer[i] = dest + dest_position;
	}

	png_read_image(png_ptr, (png_bytepp) row_ptr_buffer);
}

bool PNGReader::IsPNG(const char * bytes, unsigned int num_bytes_to_check)
{
	return png_sig_cmp((png_const_bytep) bytes, 0, num_bytes_to_check) == 0;
}


void PNGReader::PNGStreamReadFn(png_structp png_ptr, png_bytep out_bytes, png_size_t bytes_to_read)
{
	png_voidp src = png_get_io_ptr(png_ptr);
	PNGReadStream * src_data = (PNGReadStream *) src;
	src_data->Read((char *) out_bytes, bytes_to_read);
}