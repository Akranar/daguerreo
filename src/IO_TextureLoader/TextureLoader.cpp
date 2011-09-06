#include "TextureLoader.h"
#include "ExLib_RendererBase.h"
#include "ExLib_SOIL.h"
#include <Core_Exception.h>
#include <cstring>

void TextureLoader::LoadTexture1D(const char * filename, DescTexture1D & texture, FixedArray<unsigned char> & texture_data)
{

}
void TextureLoader::LoadTexture2D(const char * filename, DescTexture2D & texture, FixedArray<unsigned char> & texture_data)
{
	
	std::string file(filename);
	size_t pos = file.find('.');
	CONDITIONAL_EXCEPTION(pos == std::string::npos, "No file extension found in filename.");
	pos += 1;
	std::string extension = file.substr(pos, file.length()-pos);
	if (extension.compare("png") == 0 ||
		extension.compare("jpg") == 0 ||
		extension.compare("jpeg") == 0 ||
		extension.compare("bmp") == 0 ||
		extension.compare("tga") == 0 ||
		extension.compare("dds") == 0 ||
		extension.compare("hdr") == 0 ||
		extension.compare("psd") == 0)
	{
		int width, height, channels;
		unsigned char *data = SOIL_load_image
		(
			filename,
			&width, &height, &channels,
			SOIL_LOAD_RGBA
		);
		unsigned int format;
		/*
		switch(channels)
		{
			case 1:
				throw Exception("Format not recognized or supported.");
				break;
			case 2:
				throw Exception("Format not recognized or supported.");
				break;
			case 3:
				throw Exception("Format not recognized or supported.");
				break;
			case 4:
				format = PF_32_R8G8B8A8;
				break;
			default:
				throw Exception("Format not recognized or supported.");
				break;
		}
		*/
		format = PF_32_R8G8B8A8;
		unsigned int texture_size = width*height*4;
		texture_data.Set(texture_size);
		memcpy(texture_data.GetData(), data, texture_size);
		//texture.BindFlags = BIND_SHADER_RESOURCE;
		texture.Format = format;
		texture.Width = width;
		texture.Height = height;
		//texture.Usage = TEXTURE_USAGE_STATIC;
		//texture.MipLevels = 1;
		texture.InitialData = texture_data.GetData();

		free(data);
	}
	else
	{
		throw Exception("File extension not supported.");
	}
	
}
void TextureLoader::LoadTexture3D(const char * filename, DescTexture3D & texture, FixedArray<unsigned char> & texture_data)
{

}
void TextureLoader::LoadTextureCube(const char * filenames[6], DescTextureCube & texture, FixedArray<unsigned char> & texture_data)
{
	
	int texture_width = 0;
	int texture_height = 0;
	int texture_channels = 0;
	unsigned int pixel_format;
	unsigned int total_face_bytes = 0;
	unsigned int dest_offset = 0;
	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string file(filenames[i]);
		size_t pos = file.find('.');
		if (pos == std::string::npos)
		{
			throw Exception("No file extension found in filename.");
		}
		pos += 1;
		std::string extension = file.substr(pos, file.length()-pos);
		if (extension.compare("png") == 0 ||
			extension.compare("jpg") == 0 ||
			extension.compare("jpeg") == 0 ||
			extension.compare("bmp") == 0 ||
			extension.compare("tga") == 0 ||
			extension.compare("dds") == 0 ||
			extension.compare("hdr") == 0 ||
			extension.compare("psd") == 0)
		{
			int width, height, channels;
			unsigned char *data = SOIL_load_image
			(
				filenames[i],
				&width, &height, &channels,
				SOIL_LOAD_RGBA
			);
			unsigned int format;
			/*
			switch(channels)
			{
				case 1:
					throw Exception("Format not recognized or supported.");
					break;
				case 2:
					throw Exception("Format not recognized or supported.");
					break;
				case 3:
					throw Exception("Format not recognized or supported.");
					break;
				case 4:
					format = PF_32_R8G8B8A8;
					break;
				default:
					throw Exception("Format not recognized or supported.");
					break;
			}
			*/
			format = PF_32_R8G8B8A8;
			if (texture_width == 0 && texture_height == 0)
			{
				texture_width = width;
				texture_height = height;
				texture_channels = channels;
				pixel_format = format;

				total_face_bytes = texture_width*texture_height*4;
				texture_data.Set(total_face_bytes * 6);

				//texture.BindFlags = BIND_SHADER_RESOURCE;
				texture.Format = format;
				texture.Width = width;
				texture.Height = height;
				//texture.Usage = TEXTURE_USAGE_STATIC;
				//texture.MipLevels = 1;
				
				
			}
			else
			{
				if (texture_width != width)
				{
					throw Exception("Different Widths In Cube Map.");
				}
				if (texture_height != height)
				{
					throw Exception("Different Heights In Cube Map.");
				}
				if (texture_channels != channels)
				{
					throw Exception("Different Channels In Cube Map.");
				}
				if (pixel_format != format)
				{
					throw Exception("Different Format In Cube Map.");
				}
			}
			memcpy(texture_data.GetData() + dest_offset, data, total_face_bytes);
			texture.InitialData[i] = texture_data.GetData() + dest_offset;
			dest_offset += total_face_bytes;

			free(data);
		}
		else
		{
			throw Exception("File extension not supported.");
		}
	}
	
}
